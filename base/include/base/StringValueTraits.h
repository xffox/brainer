#ifndef BASE_STRINGVALUETRAITS_H
#define BASE_STRINGVALUETRAITS_H

#include <cstdlib>
#include <limits>
#include <sstream>
#include <string>

namespace stringvalue
{
    template<class T>
    struct StringValueTraits;

    template<>
    struct StringValueTraits<int>
    {
        static bool from(int &value, const std::string &str)
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

        static std::string to(int value)
        {
            std::stringstream stream;
            stream<<value;
            return stream.str();
        }
    };
}

#endif
