#ifndef BASE_STRINGVALUE_H
#define BASE_STRINGVALUE_H

#include <string>
#include <algorithm>
#include <locale>
#include <sstream>
#include <vector>
#include <cassert>

namespace base
{
    namespace strutil
    {
        template<typename T>
        bool fromStr(T &value, const std::string &str);
        template<>
        bool fromStr(int &value, const std::string &str);

        template<typename T>
        std::string toStr(T value)
        {
            return std::to_string(value);
        }

        template<class Iterator, class CharT>
        std::vector<std::basic_string<CharT>> split(
            Iterator begin, Iterator end, CharT sep)
        {
            std::vector<std::basic_string<CharT>> res;
            auto pos = begin;
            while(pos != end)
            {
                pos = std::find_if(pos, end, [sep](CharT c){ return c != sep; });
                if(pos == end)
                    break;
                auto endPos = std::find(pos, end, sep);
                assert(endPos - pos > 0);
                res.push_back(std::basic_string<CharT>(pos, endPos));
                pos = endPos;
            }
            return res;
        }

        template<class Iterator, class CharT>
        std::basic_string<CharT> join(
            Iterator begin, Iterator end, const std::basic_string<CharT> &sep)
        {
            std::basic_string<CharT> result;
            bool first = true;
            for(; begin != end; ++begin)
            {
                if(!first)
                {
                    result.append(sep);
                }
                else
                {
                    first = false;
                }
                result.append(*begin);
            }
            return result;
        }
    }
}

#endif
