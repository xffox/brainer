#include "LogHandler.h"

#include <xlog/xlog.hpp>

#include <cassert>

namespace bot
{
    namespace
    {
        xlog::LogLevel convertLogLevel(gloox::LogLevel level)
        {
            switch(level)
            {
            case gloox::LogLevel::LogLevelDebug:
                return xlog::LOG_LEVEL_DEBUG;
            case gloox::LogLevel::LogLevelWarning:
                return xlog::LOG_LEVEL_WARNING;
            case gloox::LogLevel::LogLevelError:
                return xlog::LOG_LEVEL_ERROR;
            default:
                assert(false);
                return xlog::LOG_LEVEL_INFO;
            }
        }
    }

    void LogHandler::handleLog(gloox::LogLevel level,
        gloox::LogArea, const std::string &message)
    {
        xlog::log().log(convertLogLevel(level),
            "xmpp", "xmpp log: %s", message.c_str());
    }
}
