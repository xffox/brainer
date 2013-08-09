#include "core/TaskLogic.h"

#include <cassert>

#include "core/ITask.h"
#include "core/ITaskGenerator.h"
#include "core/ITaskLogicWatcher.h"
#include "Stopwatch.h"

namespace core
{
    // TODO: null exception
    TaskLogic::TaskLogic(std::auto_ptr<ITaskGenerator> taskGenerator,
        ITaskLogicWatcher *watcher)
        :stopwatch(new Stopwatch()), taskGenerator(taskGenerator),
        watcher(watcher), currentTask()
    {
    }

    TaskLogic::~TaskLogic()
    {
    }

    void TaskLogic::generate()
    {
        assert(taskGenerator.get());
        ITask *const prev = currentTask.get();
        currentTask.reset(taskGenerator->generateTask().release());
        assert(currentTask.get());
        assert(stopwatch.get());
        stopwatch->reset();
        if(watcher)
            watcher->onGenerated(*currentTask, prev);
    }

    bool TaskLogic::validate(const std::string &result)
    {
        if(currentTask.get())
        {
            const bool res = currentTask->validate(result);
            if(watcher)
            {
                assert(stopwatch.get());
                watcher->onResult(*currentTask, res, stopwatch->elapsed());
            }
            return res;
        }
        // TODO: or throw exception
        return false;
    }

    void TaskLogic::describe(IRender &render)
    {
        if(currentTask.get())
            currentTask->describe(render);
        // TODO: or throw exception
    }

    long long TaskLogic::elapsed() const
    {
        assert(stopwatch.get());
        return stopwatch->elapsed();
    }
}
