#include "task/WordGenTaskGenerator.h"

#include <cstddef>
#include <algorithm>
#include <unordered_set>
#include <iterator>
#include <utility>
#include <stdexcept>
#include <fstream>
#include <locale>

#include <csv/csv.h>
#include <base/strutil.h>
#include <base/FileConfig.h>
#include <core/String.h>

#include "task/DictTask.h"
#include "task/StringCollection.h"

namespace task
{
    namespace
    {
        auto prepareParamVocabulary(const WordGenTaskGenerator::StringCol &params)
        {
            using WordSet = std::unordered_set<core::String>;
            if(params.empty())
            {
                throw std::invalid_argument(
                    "word generator invalid params");
            }
            WordSet vocabulary;
            {
                std::wifstream fin(params[0]);
                fin.imbue(std::locale(""));
                if(fin.is_open())
                {
                    csv::Csv<wchar_t> reader(fin, L'\t', L'\0', L'\\');
                    while(true)
                    {
                        const auto row = reader.row();
                        if(!row.second)
                        {
                            break;
                        }
                        if(row.first.size() != 3)
                        {
                            throw std::runtime_error(
                                "word generator invalid row format");
                        }
                        const auto &r = row.first[1];
                        auto words = base::strutil::split(
                            r.begin(), r.end(), L',');
                        std::transform(words.begin(), words.end(),
                            std::inserter(vocabulary, vocabulary.begin()),
                            [](const core::String &w) {
                                return base::strutil::trim(w);
                            });
                    }
                }
                else
                {
                    throw std::runtime_error(
                        "word generator vocabulary file open error");
                }
            }
            return StringCollection(vocabulary.begin(), vocabulary.end());
        }

        auto prepareParamLetters(const WordGenTaskGenerator::StringCol &params)
        {
            WordGenTaskGenerator::LetterMap letters;
            for(std::size_t i = 1; i < params.size(); ++i)
            {
                const auto letterValues = base::FileConfig(params[i]).read();
                for(const auto &p : letterValues)
                {
                    letters[p.first] = p.second;
                }
            }
            return letters;
        }
    }

    WordGenTaskGenerator::WordGenTaskGenerator(base::Randomizer &&random,
        const StringCollection &vocabulary, const LetterMap &letters)
        :letters(letters), random(std::move(random)),
        wordGen(this->random.diverge(), vocabulary)
    {}

    WordGenTaskGenerator::WordGenTaskGenerator(base::Randomizer &&random,
        const StringCol &params)
        :WordGenTaskGenerator(std::move(random),
            prepareParamVocabulary(params), prepareParamLetters(params))
    {}

    std::unique_ptr<core::ITask> WordGenTaskGenerator::generateTask()
    {
        const auto word = wordGen.generate();
        core::String key;
        for(std::size_t i = 0; i < word.size();)
        {
            std::size_t j = 1;
            auto iter = letters.end();
            auto prevIter = letters.end();
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
            new DictTask(random.diverge(),
                StringCollection{key}, StringCollection{word}));
    }
}
