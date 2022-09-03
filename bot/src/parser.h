#ifndef BOT_PARSER_H
#define BOT_PARSER_H

#include <string>
#include <optional>
#include <utility>

#include "Command.h"

namespace bot::parser
{
    struct Input
    {
        enum Type
        {
            TYPE_MSG,
            TYPE_CMD
        };

        Input() = default;

        Input(std::string msg)
            :type(TYPE_MSG), msg(std::move(msg)), cmd{}
        {}

        Input(Command cmd)
            :type(TYPE_CMD), msg(), cmd(std::move(cmd))
        {}

        Type type = TYPE_MSG;
        std::string msg{};
        Command cmd{};
    };

    std::optional<Input> parse(const std::string &str);
}

#endif
