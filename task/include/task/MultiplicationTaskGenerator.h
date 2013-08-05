#ifndef TASK_MULTIPLICATIONTASKGENERATOR_H
#define TASK_MULTIPLICATIONTASKGENERATOR_H

#include "core/ITaskGenerator.h"

namespace task
{
    class MultiplicationTaskGenerator: public core::ITaskGenerator
    {
    public:
        MultiplicationTaskGenerator(unsigned int seed);

        virtual std::auto_ptr<core::ITask> generateTask();
    };
}

#endif
