#ifndef BASE_STRINGVALUE_H
#define BASE_STRINGVALUE_H

#include <string>
#include <locale>
#include <sstream>

namespace base
{
    namespace strutil
    {
        template<typename T>
        bool fromStr(T &value, const std::string &str);

        template<>
        bool fromStr(int &value, const std::string &str)
        {
            const char *const s = str.c_str();
            char *endptr = 0;
            long res = std::strtol(s, &endptr, 10);
            if(endptr != 0 && *endptr == '\0')
            {
                if(res >= std::numeric_limits<int>::min() &&
                    res <= std::numeric_limits<int>::max())
                {
                    value = res;
                    return true;
                }
            }
            return false;
        }

        template<typename T>
        std::string toStr(T value)
        {
            return std::to_string(value);
        }
    }
}

#endif
