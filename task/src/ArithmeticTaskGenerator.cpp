#include "task/ArithmeticTaskGenerator.h"

#include <cstdlib>

#include "task/ArithmeticTask.h"

namespace task
{
    ArithmeticTaskGenerator::ArithmeticTaskGenerator(unsigned int seed)
    {
        srand(seed);
    }

    std::auto_ptr<core::ITask> ArithmeticTaskGenerator::generateTask()
    {
        return std::auto_ptr<core::ITask>(
            new ArithmeticTask(rand()%100, rand()%100,
                static_cast<ArithmeticTask::Operation>(rand()%2)));
    }
}
