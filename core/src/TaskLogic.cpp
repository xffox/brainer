#include "core/TaskLogic.h"

#include <cassert>

#include "core/ITask.h"
#include "core/ITaskGenerator.h"
#include "core/IRender.h"
#include "Stopwatch.h"

namespace core
{
    // TODO: null exception
    TaskLogic::TaskLogic(std::unique_ptr<ITaskGenerator> taskGenerator,
        IRender &render)
        :render(render), stopwatch(new Stopwatch()),
        taskGenerator(std::move(taskGenerator)),
        currentTask(), current(), stats()
    {
        generate();
    }

    TaskLogic::~TaskLogic()
    {
    }

    void TaskLogic::generate()
    {
        assert(stopwatch.get());
        if(!current.isNull())
        {
            current->timeUs = stopwatch->elapsed();
            stats.push_back(*current);
        }
        current = base::Nullable<Stats>(Stats());
        assert(taskGenerator.get());
        currentTask.reset(taskGenerator->generateTask().release());
        assert(currentTask.get());
        stopwatch->reset();
        currentTask->describe(render);
    }

    bool TaskLogic::validate(const String &result)
    {
        if(currentTask.get())
        {
            const bool res = currentTask->validate(result);
            if(!current.isNull())
                current->answered = res;
            if(res)
            {
                generate();
            }
            else
            {
                if(!current.isNull())
                    current->tries += 1;
                render.showInvalid(result);
            }
            return res;
        }
        // TODO: or throw exception
        return false;
    }

    void TaskLogic::skip()
    {
        if(currentTask.get())
            render.showAnswer(currentTask->answer());
        generate();
    }

    long long TaskLogic::elapsed() const
    {
        assert(stopwatch.get());
        return stopwatch->elapsed();
    }

    const TaskLogic::StatsCol &TaskLogic::getStats() const
    {
        return stats;
    }
}
