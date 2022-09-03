#include "task/MultiLetterTaskGenerator.h"

#include <utility>
#include <stdexcept>
#include <cassert>

#include "task/MultiLetterTask.h"

namespace task
{
    MultiLetterTaskGenerator::MultiLetterTaskGenerator(
        base::Randomizer &&random,
        const LetterCol &letters, std::size_t maxLength)
        :maxLength(maxLength), letters(letters), random(std::move(random))
    {
        if(maxLength == 0)
        {
            throw std::invalid_argument("zero max length");
        }
        if(this->letters.empty())
        {
            throw std::invalid_argument("empty letters");
        }
    }

    std::unique_ptr<core::ITask> MultiLetterTaskGenerator::generateTask()
    {
        assert(maxLength > 0);
        const auto sz =
            random.uniformInteger<std::size_t>(0, maxLength-1) + 1;
        core::String key;
        core::String value;
        assert(!letters.empty());
        for(std::size_t i = 0; i < sz; ++i)
        {
            const auto &letter = letters[
                random.uniformInteger<std::size_t>(0, letters.size()-1)];
            key.append(letter.first);
            value.append(letter.second);
        }
        return std::unique_ptr<MultiLetterTask>(new MultiLetterTask(
                std::move(key), std::move(value)));
    }
}
