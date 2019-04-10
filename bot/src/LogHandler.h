#ifndef BOT_LOGHANDLER_H
#define BOT_LOGHANDLER_H

#include <gloox/loghandler.h>

namespace bot
{
    class LogHandler: public gloox::LogHandler
    {
    public:
        void handleLog(gloox::LogLevel level,
            gloox::LogArea area, const std::string &message) override;
    };
}

#endif
