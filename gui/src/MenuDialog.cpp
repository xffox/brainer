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

#include "TaskDialog.h"
#include "core/ITaskProvider.h"
#include "core/ITaskGenerator.h"

namespace gui
{
    namespace
    {
        const char *TASK_PROPERTY = "task";
    }

    MenuDialog::MenuDialog(std::unique_ptr<core::ITaskProvider> taskProvider,
        QWidget *parent)
        :QDialog(parent), taskProvider(std::move(taskProvider))
    {
        ui.setupUi(this);

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
        QLayoutItem *child = 0;
        while((child = ui.taskLayout->takeAt(0)))
            delete child;
        for(QStringList::const_iterator iter = tasks.begin(),
            endIter = tasks.end(); iter != endIter; ++iter)
        {
            QPushButton *button  = new QPushButton(*iter, this);
            button->setProperty(TASK_PROPERTY, *iter);
            ui.taskLayout->addWidget(button);
            connect(button, SIGNAL(clicked()), this, SLOT(onSelected()));
        }
    }

    void MenuDialog::onSelected()
    {
        QObject *const button = sender();
        Q_ASSERT(button);
        if(taskProvider.get())
            taskDialog.setTaskGenerator(taskProvider->create(qPrintable(button->property(TASK_PROPERTY).toString())));
        taskDialog.show();
    }
}
