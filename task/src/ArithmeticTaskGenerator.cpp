#include "task/ArithmeticTaskGenerator.h"

#include <type_traits>
#include <utility>

#include "task/ArithmeticTask.h"

namespace task
{
    ArithmeticTaskGenerator::ArithmeticTaskGenerator(base::Randomizer &&random)
        :random(std::move(random))
    {}

    std::unique_ptr<core::ITask> ArithmeticTaskGenerator::generateTask()
    {
        constexpr ArithmeticTask::Value MIN_VAL = 0;
        constexpr ArithmeticTask::Value MAX_VAL = 99;
        auto generateValue = [this](){
            return random.uniformInteger(MIN_VAL, MAX_VAL);
        };
        // TODO: better enum generation
        return std::unique_ptr<core::ITask>(
            new ArithmeticTask(generateValue(), generateValue(),
                static_cast<ArithmeticTask::Operation>(
                    random.uniformInteger<std::underlying_type_t<
                        ArithmeticTask::Operation>>(0, 1))));
    }
}
