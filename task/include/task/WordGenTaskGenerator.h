#ifndef TASK_WORDGENTASKGENERATOR_H
#define TASK_WORDGENTASKGENERATOR_H

#include <string>
#include <vector>
#include <unordered_map>

#include <base/randomizer.hpp>
#include "core/ITaskGenerator.h"
#include "task/StringCollection.h"
#include "task/WordGen.h"

namespace task
{
    class WordGenTaskGenerator: public core::ITaskGenerator
    {
    public:
        using LetterMap =
            std::unordered_map<core::String, core::String>;
        using StringCol = std::vector<std::string>;

    public:
        WordGenTaskGenerator(base::Randomizer &&random,
            const StringCollection &vocabulary, const LetterMap &letters);
        WordGenTaskGenerator(base::Randomizer &&random,
            const StringCol &params);

        std::unique_ptr<core::ITask> generateTask() override;

    private:
        LetterMap letters;
        base::Randomizer random;
        WordGen wordGen;
    };
}

#endif
