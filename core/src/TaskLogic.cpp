#include "core/TaskLogic.h"

#include <cassert>
#include <limits>

#include "core/ITask.h"
#include "core/ITaskGenerator.h"
#include "core/IRender.h"
#include "Stopwatch.h"

namespace core
{
    // TODO: null exception
    TaskLogic::TaskLogic(std::unique_ptr<ITaskGenerator> taskGenerator)
        :stopwatch(new Stopwatch()),
        taskGenerator(std::move(taskGenerator)),
        currentTask(), current(), stats(), hintLevel(0)
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
        hintLevel = 0;
    }

    TaskLogic::ValidationResult TaskLogic::validate(const String &result)
    {
        if(currentTask.get())
        {
            const auto res = currentTask->validate(result);
            const auto done = currentTask->done();
            std::optional<String> answer;
            if(!current.isNull())
            {
                current->answered = res.valid;
                if(!res.valid)
                {
                    current->tries += 1;
                }
            }
            if(done)
            {
                answer = currentTask->answer();
                generate();
            }
            return {{res.valid, res.description}, answer};
        }
        // TODO: or throw exception
        return {{false, {}}, String()};
    }

    String TaskLogic::skip()
    {
        String answer;
        if(currentTask.get())
            answer = currentTask->answer();
        generate();
        return answer;
    }

    void TaskLogic::describe(IRender &render)
    {
        if(currentTask.get())
        {
            currentTask->describe(render);
        }
    }

    void TaskLogic::hint(IRender &render)
    {
        if(currentTask.get())
        {
            currentTask->hint(render, hintLevel);
            if(hintLevel < std::numeric_limits<decltype(hintLevel)>::max())
                ++hintLevel;
        }
    }

    long long TaskLogic::elapsedUs() const
    {
        assert(stopwatch.get());
        return stopwatch->elapsed();
    }

    const TaskLogic::StatsCol &TaskLogic::getStats() const
    {
        return stats;
    }
}
