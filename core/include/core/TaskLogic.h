#ifndef CORE_TASKLOGIC_H
#define CORE_TASKLOGIC_H

#include <memory>
#include <string>

namespace core
{
    class IRender;
    class ITask;
    class ITaskGenerator;
    class ITaskLogicWatcher;
    class Stopwatch;

    class TaskLogic
    {
    public:
        TaskLogic(std::auto_ptr<ITaskGenerator> taskGenerator,
            ITaskLogicWatcher *watcher = 0);
        ~TaskLogic();

        void generate();
        bool validate(const std::string &result);

        void describe(IRender &render);

        long long elapsed() const;
        
    private:
        std::auto_ptr<Stopwatch> stopwatch;

        std::auto_ptr<ITaskGenerator> taskGenerator;

        ITaskLogicWatcher *watcher;

        std::auto_ptr<ITask> currentTask;
    };
}

#endif
