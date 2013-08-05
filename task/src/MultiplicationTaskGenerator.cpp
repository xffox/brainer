#include "task/MultiplicationTaskGenerator.h"

#include <cstdlib>

#include "task/MultiplicationTask.h"

namespace task
{
    MultiplicationTaskGenerator::MultiplicationTaskGenerator(unsigned int seed)
    {
        srand(seed);
    }

    std::auto_ptr<core::ITask> MultiplicationTaskGenerator::generateTask()
    {
        return std::auto_ptr<core::ITask>(new MultiplicationTask(rand()%100, rand()%100));
    }
}
