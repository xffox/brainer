#ifndef TASK_MULTIPLICATIONTASKGENERATOR_H
#define TASK_MULTIPLICATIONTASKGENERATOR_H

#include "core/ITaskGenerator.h"

namespace task
{
    class ArithmeticTaskGenerator: public core::ITaskGenerator
    {
    public:
        ArithmeticTaskGenerator(unsigned int seed);

        virtual std::unique_ptr<core::ITask> generateTask();
    };
}

#endif
