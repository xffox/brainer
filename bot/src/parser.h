#ifndef BOT_PARSER_H
#define BOT_PARSER_H

#include <string>

#include "Command.h"

#include "base/Nullable.h"

namespace bot
{
    namespace parser
    {
        struct Input
        {
            enum Type
            {
                TYPE_MSG,
                TYPE_CMD
            };

            Input()
                :type(TYPE_MSG), msg(), cmd{}
            {}

            Input(const std::string &msg)
                :type(TYPE_MSG), msg(msg), cmd{}
            {}

            Input(const Command &cmd)
                :type(TYPE_CMD), msg(), cmd(cmd)
            {}

            Type type;
            std::string msg;
            Command cmd;
        };

        base::Nullable<Input> parse(const std::string &str);
    }
}

#endif
