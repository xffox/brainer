#include "task/HexByteTaskGenerator.h"

#include <cstdlib>

#include "task/HexByteTask.h"

namespace task
{
    HexByteTaskGenerator::HexByteTaskGenerator(unsigned int seed)
    {
        srand(seed);
    }

    std::auto_ptr<core::ITask> HexByteTaskGenerator::generateTask()
    {
        return std::auto_ptr<core::ITask>(new HexByteTask(rand()));
    }
}
