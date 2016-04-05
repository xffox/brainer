#include "ConnectionHandler.h"

#include "xlog/xlog.h"

namespace bot
{
    ConnectionHandler::ConnectionHandler(gloox::Client &client)
        :client(client)
    {}

    void ConnectionHandler::onConnect()
    {
        xlog::log().info("ConnectionHandler", "bot connected");
    }

    void ConnectionHandler::onDisconnect(gloox::ConnectionError e)
    {
        switch(e)
        {
        case gloox::ConnNoError:
            xlog::log().info("ConnectionHandler", "bot disconnected");
            break;
        case gloox::ConnStreamError:
            xlog::log().error("ConnectionHandler",
                "bot connection error: stream error");
            break;
        default:
            xlog::log().error("ConnectionHandler",
                "bot connection error: other error");
            break;
        }
    }

    bool ConnectionHandler::onTLSConnect(const gloox::CertInfo&)
    {
        return true;
    }
}
