#ifndef TASK_MULTIPLICATIONTASKGENERATOR_H
#define TASK_MULTIPLICATIONTASKGENERATOR_H

#include <base/randomizer.hpp>
#include "core/ITaskGenerator.h"

namespace task
{
    class ArithmeticTaskGenerator: public core::ITaskGenerator
    {
    public:
        ArithmeticTaskGenerator(base::Randomizer &&random);

        std::unique_ptr<core::ITask> generateTask() override;

    private:
        base::Randomizer random;
    };
}

#endif
