#include "task/HexByteTask.h"

#include <cstdlib>
#include <iomanip>
#include <limits>
#include <bitset>
#include <sstream>
#include <iomanip>

#include "core/IRender.h"
#include "util.h"

namespace task
{
    bool HexByteTask::validateBase(const core::String &result)
    {
        std::string regStr;
        if(!util::toRegularStr(regStr, result))
            return false;
        const char *const s = regStr.c_str();
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

    core::String HexByteTask::answer() const
    {
        std::wstringstream stream;
        stream<<std::hex;
        stream<<value;
        return stream.str();
    }

    void HexByteTask::describe(core::IRender &render) const
    {
        std::bitset<8> s(value);
        render.addText(util::toWString(s.to_string()));
    }

    void HexByteTask::hint(core::IRender&, std::size_t) const
    {}
}
