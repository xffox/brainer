#include "base/strutil.h"

namespace base
{
    namespace strutil
    {
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
    }
}
