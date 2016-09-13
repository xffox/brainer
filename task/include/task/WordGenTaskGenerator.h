#ifndef TASK_WORDGENTASKGENERATOR_H
#define TASK_WORDGENTASKGENERATOR_H

#include <unordered_map>

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

    public:
        WordGenTaskGenerator(unsigned int seed,
            const StringCollection &vocabulary, const LetterMap &letters);

        virtual std::unique_ptr<core::ITask> generateTask();

    private:
        LetterMap letters;
        WordGen wordGen;
        WordGen::RandomEngine eng;
    };
}

#endif
