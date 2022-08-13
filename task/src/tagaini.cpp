#include "task/tagaini.h"

#include <algorithm>
#include <stdexcept>
#include <stack>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <iterator>
#include <utility>
#include <cctype>

#include "base/strutil.h"
#include "csv/csv.h"

namespace task
{
    namespace tagaini
    {
        namespace
        {
            using Char = core::String::value_type;

            const std::vector<std::tuple<Char, Char>> BRACKETS{
                    {'[', ']'},
                    {'(', ')'},
                    {'{', '}'},
                };

            auto makeReverseMap(
                const std::vector<std::tuple<Char, Char>> &pairs)
            {
                std::unordered_map<Char, Char> result;
                std::transform(std::begin(pairs), std::end(pairs),
                    std::inserter(result, std::end(result)),
                    [](const auto &p){
                            return std::make_pair(std::get<1>(p),
                                std::get<0>(p));
                        });
                return result;
            }

            auto makeDirectSet(
                const std::vector<std::tuple<Char, Char>> &pairs)
            {
                std::unordered_set<Char> result;
                std::transform(std::begin(pairs), std::end(pairs),
                    std::inserter(result, std::end(result)),
                    [](const auto &p){
                            return std::get<0>(p);
                        });
                return result;
            }

            const auto REVERSE_BRACKETS = makeReverseMap(BRACKETS);
            const auto OPEN_BRACKETS = makeDirectSet(BRACKETS);

            core::String removeBrackets(const core::String &str)
            {
                std::stack<Char> brackets;
                core::String result;
                for(auto c : str)
                {
                    auto openIter = OPEN_BRACKETS.find(c);
                    if(openIter != std::end(OPEN_BRACKETS))
                    {
                        brackets.push(c);
                        continue;
                    }
                    auto revIter = REVERSE_BRACKETS.find(c);
                    if(revIter != std::end(REVERSE_BRACKETS))
                    {
                        if(brackets.empty() ||
                            (brackets.top() != revIter->second))
                        {
                            return str;
                        }
                        brackets.pop();
                        continue;
                    }
                    if(brackets.empty())
                    {
                        result.push_back(c);
                    }
                }
                if(!brackets.empty())
                {
                    return str;
                }
                return result;
            }
        }

        TaskCollection readCollection(std::wistream &stream)
        {
            struct Trimmer
            {
                bool operator()(core::String::value_type ch) const
                {
                    return std::iswspace(ch) || ch == L'.';
                }
            };
            TaskCollection result;
            csv::Csv<wchar_t> reader(stream, L'\t', L'\0', L'\\');
            while(true)
            {
                const auto row = reader.row();
                if(!row.second)
                    break;
                if(row.first.size() != 3)
                    throw std::runtime_error("invalid row format");
                auto values = base::strutil::split(
                    row.first[2].begin(), row.first[2].end(), L',');
                std::transform(values.begin(), values.end(), values.begin(),
                    [](const decltype(values)::value_type &str) {
                        return base::strutil::trim(
                            removeBrackets(str), Trimmer());
                    });
                auto prons = base::strutil::split(
                    row.first[1].begin(), row.first[1].end(), L',');
                result.push_back(std::make_tuple(row.first[0], prons, values));
            }
            return result;
        }
    }
}
