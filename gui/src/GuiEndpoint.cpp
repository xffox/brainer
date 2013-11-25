#include "GuiEndpoint.h"

#include <QStringList>
#include <QString>

#include "core/message/TasksMessage.h"
#include "core/message/DescribeMessage.h"
#include "core/message/ResultMessage.h"
#include "core/message/StartTaskMessage.h"
#include "core/message/ValidateMessage.h"
#include "core/message/SkipMessage.h"
#include "core/message/ElapsedMessage.h"
#include "core/message/StatsMessage.h"

namespace gui
{
    GuiEndpoint::GuiEndpoint()
    {
        connectToSignals();
    }

    void GuiEndpoint::handle(const core::message::TasksMessage &message)
    {
        const core::message::TasksMessage::StringSet &tasks =
            message.getTasks();
        QStringList names;
        for(core::message::TasksMessage::StringSet::const_iterator iter =
            tasks.begin(), endIter = tasks.end(); iter != endIter; ++iter)
            names.push_back(QString(iter->c_str()));
        menuDialog.showTasks(names);
    }

    void GuiEndpoint::handle(const core::message::DescribeMessage &message)
    {
        taskId = message.getTaskId();
        taskDialog.setTask(QString(message.getDescription().c_str()));
    }

    void GuiEndpoint::handle(const core::message::ResultMessage &message)
    {
        taskDialog.setValidity(message.isValid());
    }

    void GuiEndpoint::handle(const core::message::StatsMessage &message)
    {
        taskDialog.setStats(message.getStats());
    }

    void GuiEndpoint::handle(const core::message::ElapsedMessage &message)
    {
        taskDialog.setElapsed(message.getElapsed());
    }

    void GuiEndpoint::started()
    {
        menuDialog.show();
    }

    void GuiEndpoint::stopped()
    {
    }

    void GuiEndpoint::onSelected(const QString &task)
    {
        send(core::message::StartTaskMessage(qPrintable(task)));
        taskDialog.show();
    }

    void GuiEndpoint::onEntered(const QString &value)
    {
        send(core::message::ValidateMessage(taskId, qPrintable(value)));
    }

    void GuiEndpoint::onSkipped()
    {
        send(core::message::SkipMessage(taskId));
    }

    void GuiEndpoint::onTimed()
    {
        send(core::message::ElapsedMessage(taskId));
    }

    void GuiEndpoint::connectToSignals()
    {
        connect(&menuDialog, SIGNAL(selected(const QString&)), this,
            SLOT(onSelected(const QString&)));
        connect(&taskDialog, SIGNAL(entered(const QString&)), this,
            SLOT(onEntered(const QString&)));
        connect(&taskDialog, SIGNAL(skipped()), this, SLOT(onSkipped()));
        connect(&taskDialog, SIGNAL(timed()), this, SLOT(onTimed()));
    }
}
