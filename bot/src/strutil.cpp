#include "strutil.h"

#include <codecvt>
#include <locale>

namespace strutil
{
    std::string fromCoreString(const core::String &str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> convert("???");
        return convert.to_bytes(str);
    }

    core::String toCoreString(const std::string &str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> convert("???");
        return convert.from_bytes(str);
    }
}
