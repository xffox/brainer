#ifndef BOT_STRUTIL_H
#define BOT_STRUTIL_H

#include <string>
#include "core/String.h"

namespace strutil
{
    std::string fromCoreString(const core::String &str);
    core::String toCoreString(const std::string &str);
}

#endif
