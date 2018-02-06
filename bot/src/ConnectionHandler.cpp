#include "ConnectionHandler.h"

#include <cassert>

#include "xlog/xlog.h"
#include "MUCHandler.h"

namespace bot
{
    ConnectionHandler::ConnectionHandler(gloox::Client &client,
        task::TaskProvider &taskProvider,
        const std::string &roomJid)
        :roomJid(roomJid), client(client), taskProvider(taskProvider),
        mucHandler(), messageHandlers()
    {
        client.registerConnectionListener(this);
        client.registerMessageSessionHandler(this);
    }

    ConnectionHandler::~ConnectionHandler()
    {
        client.removeConnectionListener(this);
    }

    void ConnectionHandler::onConnect()
    {
        xlog::log().info("ConnectionHandler", "bot connected");
        joinRoom();
    }

    void ConnectionHandler::onDisconnect(gloox::ConnectionError e)
    {
        leaveRoom();
        messageHandlers.clear();
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

    void ConnectionHandler::onResourceBindError(const gloox::Error*)
    {
        client.disconnect();
    }

    void ConnectionHandler::onSessionCreateError(const gloox::Error*)
    {
        client.disconnect();
    }

    void ConnectionHandler::handleMessageSession(
        gloox::MessageSession *session)
    {
        assert(session);
        std::shared_ptr<MessageHandler> handler(
            new MessageHandler(client, session, taskProvider));
        try
        {
            messageHandlers.push_back(handler);
        }
        catch(const std::exception &exc)
        {
            throw exc;
        }
    }

    void ConnectionHandler::joinRoom()
    {
        leaveRoom();
        if(roomJid.empty())
            return;
        xlog::log().info("ConnectionHandler", "joining room: '%s'",
            roomJid.c_str());
        mucHandler.reset(new MUCHandler(client, roomJid, taskProvider));
    }

    void ConnectionHandler::leaveRoom()
    {
        if(mucHandler.get())
        {
            xlog::log().info("ConnectionHandler", "leaving room: '%s'",
                roomJid.c_str());
            mucHandler.reset();
        }
    }
}
