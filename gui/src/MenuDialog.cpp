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

namespace gui
{
    namespace
    {
        const char *TASK_PROPERTY = "task";
    }

    MenuDialog::MenuDialog(QWidget *parent)
        :QDialog(parent)
    {
        ui.setupUi(this);
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
        emit selected(button->property(TASK_PROPERTY).toString());
    }
}
