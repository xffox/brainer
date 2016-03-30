#include "ConnectionHandler.h"

namespace bot
{
    void ConnectionHandler::onConnect()
    {
    }

    void ConnectionHandler::onDisconnect(gloox::ConnectionError)
    {
    }

    bool ConnectionHandler::onTLSConnect(const gloox::CertInfo&)
    {
        return true;
    }
}
