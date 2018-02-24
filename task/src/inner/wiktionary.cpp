#include "task/inner/wiktionary.h"

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <iterator>
#include <cwctype>
#include <cmath>
#include <cassert>

#include "csv/csv.h"

namespace task
{
namespace inner
{
namespace wiktionary
{
    namespace
    {
        constexpr std::size_t COLUMNS = 2;
        constexpr double DROP_SIMILARITY = 0.7;
        constexpr std::size_t MIN_WORDS = 3;
        constexpr std::size_t MIN_LINES = 2;
    }

    core::String prepareDescription(const core::String &descr,
        const core::String &term)
    {
        core::String result;
        core::String lowerTerm;
        std::transform(std::begin(term), std::end(term),
            std::back_inserter(lowerTerm),
            [](core::String::value_type v){
                return std::towlower(v);
            });
        bool first = true;
        const auto descrEndIter = end(descr);
        const std::size_t dropLength = term.size()*DROP_SIMILARITY+1;
        assert(dropLength <= term.size());
        auto iter = begin(descr);
        std::size_t lines = 0;
        while(iter < descrEndIter)
        {
            size_t words = 0;
            bool similar = false;
            const auto lineBeginPos = iter;
            const auto lineEndPos = std::find(iter, descrEndIter, L'\n');
            while(iter < lineEndPos)
            {
                const auto wordBeginPos = std::find_if(iter, lineEndPos,
                    [](core::String::value_type v){
                        return std::iswalnum(v);
                    });
                iter = wordBeginPos;
                auto intermIter = begin(lowerTerm);
                const auto termDropIter = intermIter +
                    std::min(static_cast<std::size_t>(lineEndPos-iter),
                        dropLength);
                while(intermIter < termDropIter &&
                    std::iswalnum(*iter) &&
                    static_cast<wint_t>(*intermIter) == std::towlower(*iter))
                {
                    ++intermIter;
                    ++iter;
                }
                if(static_cast<std::size_t>(intermIter-begin(lowerTerm)) ==
                    dropLength)
                {
                    similar = true;
                    break;
                }
                iter = std::find_if(iter, lineEndPos,
                    [](core::String::value_type v){
                        return !std::iswalnum(v);
                    });
                if(wordBeginPos < iter)
                {
                    ++words;
                }
            }
            if(!similar && words >= MIN_WORDS)
            {
                if(!first)
                {
                    result += L'\n';
                }
                else
                {
                    first = false;
                }
                result.append(lineBeginPos, lineEndPos);
                ++lines;
            }
            iter = lineEndPos;
            if(iter < descrEndIter)
                ++iter;
        }
        if(lines >= MIN_LINES)
            return result;
        return core::String();
    }

    TaskCollection readWiktionaryDefinitions(std::wistream &stream)
    {
        TaskCollection result;
        csv::Csv<wchar_t> reader(stream, L',', L'"', L'\0');
        while(true)
        {
            auto row = reader.row();
            if(!row.second)
                break;
            if(row.first.size() != COLUMNS)
                throw std::runtime_error("invalid row format");
            if(!suitableTerm(row.first[0]))
            {
                continue;
            }
            auto preparedDescription = prepareDescription(
                row.first[1], row.first[0]);
            if(!preparedDescription.empty())
            {
                result.emplace_back(
                    StringCollection{std::move(row.first[0])},
                    StringCollection{std::move(preparedDescription)});
            }
        }
        return result;
    }

    bool suitableTerm(const core::String &term)
    {
        return std::all_of(
            std::begin(term), std::end(term),
                [](core::String::value_type v){
                    if(std::iswalpha(v))
                    {
                        const auto lv = std::towlower(v);
                        return lv >= L'a' && lv <= L'z';
                    }
                    else
                    {
                        return false;
                    }
            });
    }
}
}
}
