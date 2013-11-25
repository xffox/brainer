#include "core/CoreEndpoint.h"

#include <algorithm>
#include <cassert>
#include <exception>

#include "core/message/StartTaskMessage.h"
#include "core/message/StopTaskMessage.h"
#include "core/message/ValidateMessage.h"
#include "core/message/DescribeMessage.h"
#include "core/message/TasksMessage.h"
#include "core/message/SkipMessage.h"
#include "core/message/ResultMessage.h"
#include "core/message/StatsMessage.h"
#include "core/message/ElapsedMessage.h"
#include "core/Stats.h"
#include "core/TaskLogic.h"
#include "core/ITaskGenerator.h"
#include "core/IRender.h"
#include "core/TaskId.h"
#include "core/ITaskProvider.h"
#include "core/StatsTaskLogicWatcher.h"

namespace core
{
    namespace
    {
        double average(
            const core::StatsTaskLogicWatcher::TimeCollection &times)
        {
            if(!times.empty())
            {
                return static_cast<double>(std::accumulate(times.begin(),
                        times.end(), 0))/times.size();
            }
            return 0.0;
        }
    }

    namespace
    {
        class Render: public IRender
        {
        public:
            virtual void addText(const std::string &str)
            {
                this->str += str;
            }

            const std::string &getStr() const
            {
                return str;
            }

        private:
            std::string str;
        };
    }

    namespace
    {
        const TaskId TASK_ID = 1;
    }

    CoreEndpoint::CoreEndpoint(std::auto_ptr<ITaskProvider> taskProvider)
        :taskProvider(taskProvider), statsWatcher(), taskLogic()
    {
        // throw exception on invalid taskProvider
    }

    CoreEndpoint::~CoreEndpoint()
    {
    }

    void CoreEndpoint::started()
    {
        assert(taskProvider.get());
        message::TasksMessage tasks;
        tasks.getTasks() = taskProvider->getTasks();
        send(tasks);
    }

    void CoreEndpoint::stopped()
    {
    }

    void CoreEndpoint::handle(const message::StartTaskMessage &message)
    {
        try
        {
            taskLogic.reset();
            statsWatcher.reset(new StatsTaskLogicWatcher());
            taskLogic.reset(new TaskLogic(createGenerator(message.getTask()),
                    statsWatcher.get()));
            generate();
            stats();
        }
        catch(const std::exception&)
        {
        }
    }

    void CoreEndpoint::handle(const message::StopTaskMessage &message)
    {
        taskLogic.reset();
    }

    void CoreEndpoint::handle(const message::ValidateMessage &message)
    {
        if(taskLogic.get())
        {
            const bool valid = taskLogic->validate(message.getValue());
            send(message::ResultMessage(TASK_ID, message.getValue(),
                    valid));
            if(valid)
                generate();
            stats();
        }
    }

    void CoreEndpoint::handle(const message::SkipMessage&)
    {
        generate();
        stats();
    }

    void CoreEndpoint::handle(const message::ElapsedMessage&)
    {
        // TODO: error
        if(taskLogic.get())
            send(message::ElapsedMessage(TASK_ID).setElapsed(
                    taskLogic->elapsed()));
    }

    std::auto_ptr<ITaskGenerator> CoreEndpoint::createGenerator(
        const std::string &task) const
    {
        assert(taskProvider.get());
        return taskProvider->create(task);
    }

    void CoreEndpoint::generate()
    {
        assert(taskLogic.get());
        Render render;
        taskLogic->generate();
        taskLogic->describe(render);
        send(message::DescribeMessage(TASK_ID, render.getStr()));
    }

    void CoreEndpoint::stats()
    {
        // TODO: error
        if(statsWatcher.get())
            send(message::StatsMessage(TASK_ID,
                    Stats().setTaskCount(statsWatcher->getTasksCount())
                    .setValidCount(statsWatcher->getValidCount())
                    .setTriesCount(statsWatcher->getTriesCount())
                    .setAverageTime(average(statsWatcher->getValidTimes()))));
    }
}
