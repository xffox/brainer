#include "util.h"

#include <sstream>
#include <cwctype>

namespace task
{
    namespace util
    {
        bool toRegularStr(std::string &result, const core::String &str)
        {
            result.clear();
            for(auto v : str)
            {
                if(v >= 128)
                    return false;
                result.push_back(v);
            }
            return true;
        }

        core::String toWString(const std::string &str)
        {
            std::wstringstream stream;
            stream<<str.c_str();
            return stream.str();
        }

        core::String tolower(const core::String &str)
        {
            core::String res;
            std::transform(str.begin(), str.end(),
                std::back_inserter(res),
                [](wchar_t c){return std::towlower(c);});
            return res;
        }
    }
}
