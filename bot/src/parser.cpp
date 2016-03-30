#include "parser.h"

#include <algorithm>
#include <cstddef>
#include <cassert>

#include "StringList.h"

namespace bot
{
    namespace parser
    {
        namespace
        {
            constexpr char COMMAND_START = '.';

            template<class Iterator>
            inline Iterator findNonSep(Iterator pos, Iterator end, char sep)
            {
                return std::find_if(pos, end,
                    [sep](char c){ return c != sep; });
            }

            template<class Iterator>
            StringList split(Iterator begin, Iterator end, char sep = ' ')
            {
                StringList res;
                auto pos = begin;
                while(pos != end)
                {
                    pos = findNonSep(pos, end, sep);
                    if(pos == end)
                        break;
                    auto endPos = std::find(pos, end, sep);
                    assert(endPos - pos > 0);
                    res.push_back(std::string(pos, endPos));
                    pos = endPos;
                }
                return res;
            }
        }

        base::Nullable<Input> parse(const std::string &str)
        {
            auto iter = findNonSep(str.begin(), str.end(), ' ');
            if(iter != str.end() && *iter == COMMAND_START)
            {
                const auto words = split(iter, str.end(), ' ');
                assert(!words[0].empty());
                if(words[0].size() == 1)
                    return base::Nullable<Input>();
                return base::Nullable<Input>(Input(
                        Command{words[0].substr(1),
                        StringList(words.begin()+1, words.end())}));
            }
            else
            {
                return base::Nullable<Input>(Input(str));
            }
        }
    }
}
