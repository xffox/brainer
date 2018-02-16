#include "task/inner/wiktionary.h"

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <iterator>
#include <cwctype>

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
    }

    core::String prepareDescription(const core::String &descr,
        const core::String &term)
    {
        std::size_t pos = 0;
        core::String result;
        bool first = true;
        const auto descrSz = descr.size();
        const auto termSz = term.size();
        while(pos < descrSz)
        {
            auto eolPos = descr.find(L'\n', pos);
            if(eolPos == std::string::npos)
                eolPos = descrSz;
            size_t words = 0;
            bool similar = false;
            const auto lineBeginPos = pos;
            while(pos < eolPos)
            {
                while(pos < eolPos && !std::iswalnum(descr[pos]))
                    ++pos;
                const auto wordBeginPos = pos;
                bool interm = true;
                size_t intermPos = 0;
                while(pos < eolPos && std::iswalnum(descr[pos]))
                {
                    if(interm)
                    {
                        if(intermPos < termSz &&
                            std::towlower(term[intermPos]) ==
                            std::towlower(descr[pos]))
                        {
                            ++intermPos;
                        }
                        else
                        {
                            interm = false;
                        }
                    }
                    ++pos;
                }
                if(wordBeginPos < pos)
                {
                    if(static_cast<double>(intermPos)/(pos-wordBeginPos) >=
                        DROP_SIMILARITY)
                    {
                        similar = true;
                        break;
                    }
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
                result.append(std::begin(descr)+lineBeginPos,
                    std::begin(descr)+eolPos);
            }
            pos = eolPos;
            if(pos < descrSz)
                ++pos;
        }
        return result;
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
