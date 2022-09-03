#ifndef TASK_DICTTASKGENERATOR_H
#define TASK_DICTTASKGENERATOR_H

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <cstddef>

#include <base/randomizer.hpp>
#include "core/ITaskGenerator.h"
#include "core/String.h"
#include "task/StringCollection.h"
#include "IndexGenerator.h"

namespace core
{
    class IDictProvider;
}

namespace task
{
    class DictTaskGenerator: public core::ITaskGenerator
    {
    public:
        using TaskCollection =
            std::vector<std::pair<StringCollection, StringCollection>>;

    public:
        DictTaskGenerator(base::Randomizer &&random,
            const TaskCollection &tasks, bool reversed = false);
        ~DictTaskGenerator() override;

        std::unique_ptr<core::ITask> generateTask() override;

    private:
        TaskCollection tasks;
        bool reversed;
        base::Randomizer random;
        IndexGenerator indexGenerator;
    };
}

#endif
