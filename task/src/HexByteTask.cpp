#include "task/HexByteTask.h"

#include <cstdlib>
#include <string>
#include <optional>
#include <iomanip>
#include <limits>
#include <bitset>
#include <sstream>
#include <iomanip>

#include "core/IRender.h"
#include "util.h"

namespace task
{
    namespace
    {
        std::optional<unsigned char> parseHex(const std::string &str)
        {
            constexpr int BASE = 16;
            try
            {
                std::size_t pos{};
                const auto res = stoull(str, &pos, BASE);
                if(pos == str.size() &&
                    res <= std::numeric_limits<unsigned char>::max())
                {
                    return res;
                }
            }
            catch(const std::exception&)
            {
            }
            return std::nullopt;
        }
    }

    bool HexByteTask::validateBase(const core::String &result)
    {
        std::string regStr;
        if(!util::toRegularStr(regStr, result))
        {
            return false;
        }
        if(const auto maybeRes = parseHex(regStr))
        {
            return *maybeRes == value;
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
        constexpr std::size_t BITS = 8;
        std::bitset<BITS> s(value);
        render.addText(util::toWString(s.to_string()));
    }

    void HexByteTask::hint(core::IRender&, std::size_t)
    {}
}
