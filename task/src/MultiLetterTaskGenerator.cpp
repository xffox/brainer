#include "task/MultiLetterTaskGenerator.h"

#include <cassert>
#include <stdexcept>

#include "task/MultiLetterTask.h"

namespace task
{
    MultiLetterTaskGenerator::MultiLetterTaskGenerator(unsigned int seed,
        const LetterCol &letters, std::size_t maxLength)
        :maxLength(maxLength), letters(letters)
    {
        if(maxLength == 0)
            throw std::invalid_argument("zero max length");
        if(this->letters.empty())
            throw std::invalid_argument("empty letters");
        srand(seed);
    }

    std::unique_ptr<core::ITask> MultiLetterTaskGenerator::generateTask()
    {
        const std::size_t sz = rand()%maxLength + 1;
        core::String key;
        core::String value;
        assert(!letters.empty());
        for(std::size_t i = 0; i < sz; ++i)
        {
            const auto &letter = letters[rand()%letters.size()];
            key.append(letter.first);
            value.append(letter.second);
        }
        return std::unique_ptr<MultiLetterTask>(new MultiLetterTask(
                std::move(key), std::move(value)));
    }
}
