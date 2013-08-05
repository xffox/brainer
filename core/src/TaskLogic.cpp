#include "core/TaskLogic.h"

#include <cassert>

#include "core/ITask.h"
#include "core/ITaskGenerator.h"
#include "Stopwatch.h"

namespace core
{
    // TODO: null exception
    TaskLogic::TaskLogic(std::auto_ptr<ITaskGenerator> taskGenerator)
        :stopwatch(new Stopwatch()), taskGenerator(taskGenerator),
        currentTask()
    {
    }

    TaskLogic::~TaskLogic()
    {
    }

    void TaskLogic::generate()
    {
        assert(taskGenerator.get());
        currentTask.reset(taskGenerator->generateTask().release());
        assert(stopwatch.get());
        stopwatch->reset();
    }

    bool TaskLogic::validate(const std::string &result)
    {
        if(currentTask.get())
            return currentTask->validate(result);
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
