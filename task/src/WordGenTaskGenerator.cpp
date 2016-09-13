#include "task/WordGenTaskGenerator.h"

#include <cstddef>

#include "task/DictTask.h"

namespace task
{
    WordGenTaskGenerator::WordGenTaskGenerator(unsigned int seed,
        const StringCollection &vocabulary, const LetterMap &letters)
        :letters(letters), wordGen(vocabulary), eng(seed)
    {
    }

    std::unique_ptr<core::ITask> WordGenTaskGenerator::generateTask()
    {
        const auto word = wordGen.generate(eng);
        core::String key;
        for(std::size_t i = 0; i < word.size();)
        {
            std::size_t j = 1;
            LetterMap::const_iterator iter = letters.end();
            LetterMap::const_iterator prevIter = letters.end();
            while(i+j <= word.size() &&
                (iter = letters.find(word.substr(i, j))) != letters.end())
            {
                prevIter = iter;
                j += 1;
            }
            if(prevIter != letters.end())
            {
                key.append(prevIter->second);
                i += prevIter->first.size();
            }
            else
            {
                key.push_back(word[i]);
                i += 1;
            }
        }
        return std::unique_ptr<core::ITask>(
            new DictTask(StringCollection{key}, StringCollection{word}));
    }
}
