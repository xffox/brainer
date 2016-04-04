#include "task/HexByteTaskGenerator.h"

#include <cstdlib>

#include "task/HexByteTask.h"

namespace task
{
    HexByteTaskGenerator::HexByteTaskGenerator(unsigned int seed)
    {
        srand(seed);
    }

    std::unique_ptr<core::ITask> HexByteTaskGenerator::generateTask()
    {
        return std::unique_ptr<core::ITask>(new HexByteTask(rand()));
    }
}
