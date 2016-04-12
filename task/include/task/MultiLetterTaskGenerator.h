#ifndef TASK_MULTILETTERTASKGENERATOR_H
#define TASK_MULTILETTERTASKGENERATOR_H

#include <vector>
#include <utility>
#include <cstddef>

#include "core/ITaskGenerator.h"
#include "core/String.h"

namespace task
{
    class MultiLetterTaskGenerator: public core::ITaskGenerator
    {
    public:
        using LetterCol = std::vector<std::pair<core::String, core::String>>;

    public:
        MultiLetterTaskGenerator(unsigned int seed, const LetterCol &letters,
            std::size_t maxLength = 8);

    public:
        virtual std::unique_ptr<core::ITask> generateTask();

    private:
        std::size_t maxLength;
        LetterCol letters;
    };
}

#endif
