#include "task/HexByteTask.h"

#include <cstdlib>
#include <iomanip>
#include <limits>
#include <bitset>

#include "core/IRender.h"

namespace task
{
    bool HexByteTask::validate(const ValueType &result)
    {
        const std::string str = result;
        const char *const s = str.c_str();
        char *endptr = 0;
        unsigned long res = std::strtoul(s, &endptr, 16);
        if(endptr != 0 && *endptr == '\0')
        {
            if(res <= std::numeric_limits<unsigned char>::max())
            {
                return res == value;
            }
        }
        return false;
    }

    void HexByteTask::describe(core::IRender &render)
    {
        std::bitset<8> s(value);
        render.addText(s.to_string());
    }
}
