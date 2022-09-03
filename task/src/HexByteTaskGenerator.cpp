#include "task/HexByteTaskGenerator.h"

#include <utility>

#include "task/HexByteTask.h"

namespace task
{
    HexByteTaskGenerator::HexByteTaskGenerator(base::Randomizer &&random)
        :random(std::move(random))
    {}

    std::unique_ptr<core::ITask> HexByteTaskGenerator::generateTask()
    {
        return std::unique_ptr<core::ITask>(new HexByteTask(
                random.uniformInteger<HexByteTask::Value>()));
    }
}
