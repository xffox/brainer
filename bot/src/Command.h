#ifndef BOT_COMMAND_H
#define BOT_COMMAND_H

#include <string>

#include "StringList.h"

namespace bot
{
    struct Command
    {
        std::string cmd;
        StringList args;
    };
}

#endif
