#include "parser.h"

#include <algorithm>
#include <cstddef>
#include <cassert>

#include "base/strutil.h"
#include "StringList.h"

namespace bot::parser
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
    }

    std::optional<Input> parse(const std::string &str)
    {
        auto iter = findNonSep(str.begin(), str.end(), ' ');
        if(iter != str.end() && *iter == COMMAND_START)
        {
            const auto words = base::strutil::split(iter, str.end(), ' ');
            assert(!words[0].empty());
            if(words[0].size() == 1)
            {
                return std::nullopt;
            }
            return Input(Command{words[0].substr(1),
                StringList(words.begin()+1, words.end())});
        }
        return Input(str);
    }
}
