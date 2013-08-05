#ifndef CORE_TASKLOGIC_H
#define CORE_TASKLOGIC_H

#include <memory>
#include <string>

namespace core
{
    class ITask;
    class IRender;
    class ITaskGenerator;
    class Stopwatch;

    class TaskLogic
    {
    public:
        TaskLogic(std::auto_ptr<ITaskGenerator> taskGenerator);
        ~TaskLogic();

        void generate();
        bool validate(const std::string &result);

        void describe(IRender &render);

        long long elapsed() const;
        
    private:
        std::auto_ptr<Stopwatch> stopwatch;

        std::auto_ptr<ITaskGenerator> taskGenerator;
        std::auto_ptr<ITask> currentTask;
    };
}

#endif
