#ifndef TASK_HEXBYTETASKGENERATOR_H
#define TASK_HEXBYTETASKGENERATOR_H

#include <base/randomizer.hpp>
#include "core/ITaskGenerator.h"

namespace task
{
    class HexByteTaskGenerator: public core::ITaskGenerator
    {
    public:
        HexByteTaskGenerator(base::Randomizer &&random);

        std::unique_ptr<core::ITask> generateTask() override;

    private:
        base::Randomizer random;
    };
}

#endif
