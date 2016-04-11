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
        room(), mucHandler()
    {}

    ConnectionHandler::~ConnectionHandler()
    {
    }

    void ConnectionHandler::onConnect()
    {
        xlog::log().info("ConnectionHandler", "bot connected");
        joinRoom();
    }

    void ConnectionHandler::onDisconnect(gloox::ConnectionError e)
    {
        leaveRoom();
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

    void ConnectionHandler::joinRoom()
    {
        leaveRoom();
        if(roomJid.empty())
            return;
        xlog::log().info("ConnectionHandler", "joining room: '%s'",
            roomJid.c_str());
        room.reset(new gloox::MUCRoom(&client, roomJid, nullptr));
        mucHandler.reset(new MUCHandler(*room, taskProvider));
        room->registerMUCRoomHandler(mucHandler.get());
        room->registerMUCRoomConfigHandler(mucHandler.get());
        room->join();
    }

    void ConnectionHandler::leaveRoom()
    {
        if(mucHandler.get())
        {
            xlog::log().info("ConnectionHandler", "leaving room: '%s'",
                roomJid.c_str());
            assert(room.get());
            room->removeMUCRoomHandler();
            room->removeMUCRoomConfigHandler();
            room->leave();
            mucHandler.reset();
            room.reset();
        }
    }
}
