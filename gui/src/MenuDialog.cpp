#include "MenuDialog.h"

// TODO: cleanup
#include <algorithm>
#include <cassert>
#include <ctime>
#include <exception>
#include <iterator>
#include <memory>
#include <utility>

#include <QPushButton>
#include <QMessageBox>

#include "TaskDialog.h"
#include "core/ITaskProvider.h"
#include "core/ITaskGenerator.h"

namespace gui
{
    MenuDialog::MenuDialog(std::unique_ptr<core::ITaskProvider> taskProvider,
        QWidget *parent)
        :QDialog(parent), taskProvider(std::move(taskProvider))
    {
        ui.setupUi(this);

        connectToSignals();

        if(this->taskProvider.get())
        {
            const core::ITaskProvider::StringSet tasks = this->taskProvider->getTasks();
            QStringList names;
            for(core::ITaskProvider::StringSet::const_iterator iter =
                tasks.begin(), endIter = tasks.end(); iter != endIter; ++iter)
                names.push_back(QString(iter->c_str()));
            showTasks(names);
        }
    }

    MenuDialog::~MenuDialog()
    {
    }

    void MenuDialog::showTasks(const QStringList &tasks)
    {
        ui.taskList->clear();
        for(QStringList::const_iterator iter = tasks.begin(),
            endIter = tasks.end(); iter != endIter; ++iter)
        {
            ui.taskList->addItem(*iter);
        }
    }

    void MenuDialog::onSelected(QListWidgetItem *item)
    {
        if(taskProvider.get())
        {
            try
            {
                taskDialog.setTaskGenerator(taskProvider->create(
                        qPrintable(item->text())));
                taskDialog.show();
            }
            catch(const std::exception &exc)
            {
                QMessageBox::critical(this, "error", exc.what());
            }
        }
    }

    void MenuDialog::onStart() {
        auto *const currentItem = ui.taskList->currentItem();
        if(currentItem)
        {
            onSelected(currentItem);
        }
    }

    void MenuDialog::connectToSignals()
    {
        connect(ui.taskList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(onSelected(QListWidgetItem*)));
        connect(ui.startButton, SIGNAL(clicked()),
            this, SLOT(onStart()));
    }
}
