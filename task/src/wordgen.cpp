#include "wordgen.h"

#include <sstream>
#include <fstream>
#include <unordered_set>
#include <stdexcept>
#include <locale>
#include <cwctype>
#include <algorithm>
#include <cstdlib>
#include <cstddef>
#include <random>

#include "csv/csv.h"
#include "base/strutil.h"
#include "base/FileConfig.h"

#include "core/String.h"
#include "task/WordGenTaskGenerator.h"
#include "task/StringCollection.h"

namespace task
{
    namespace wordgen
    {
        std::unique_ptr<core::ITaskGenerator> createTaskGenerator(
            const TaskProvider::StringCol &params)
        {
            if(params.empty())
                    throw std::invalid_argument(
                        "word generator invalid params");
            using WordSet = std::unordered_set<core::String>;
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
                            break;
                        if(row.first.size() != 3)
                            throw std::runtime_error(
                                "word generator invalid row format");
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
            WordGenTaskGenerator::LetterMap letters;
            for(std::size_t i = 1; i < params.size(); ++i)
            {
                const auto letterValues = base::FileConfig(params[i]).read();
                for(const auto &p : letterValues)
                {
                    letters[p.first] = p.second;
                }
            }
            return std::unique_ptr<core::ITaskGenerator>(
                new WordGenTaskGenerator(
                    std::random_device()(),
                    StringCollection(vocabulary.begin(), vocabulary.end()),
                    letters));
        }
    }
}
