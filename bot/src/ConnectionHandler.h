#ifndef BOT_CONNECTIONHANDLER_H
#define BOT_CONNECTIONHANDLER_H

#include <memory>
#include <string>

#include <gloox/connectionlistener.h>
#include <gloox/mucroom.h>
#include <gloox/client.h>

#include "task/TaskProvider.h"

namespace bot
{
    class MUCHandler;

    class ConnectionHandler: public gloox::ConnectionListener
    {
    public:
        ConnectionHandler(gloox::Client &client,
            task::TaskProvider &taskProvider,
            const std::string &roomJid = std::string());
        virtual ~ConnectionHandler();

    protected:
        virtual void onConnect();
        virtual void onDisconnect(gloox::ConnectionError e);
        virtual bool onTLSConnect(const gloox::CertInfo& info);

    private:
        void joinRoom();
        void leaveRoom();
        
    private:
        std::string roomJid;
        gloox::Client &client;
        task::TaskProvider &taskProvider;

        std::unique_ptr<gloox::MUCRoom> room;
        std::unique_ptr<MUCHandler> mucHandler;
    };
}

#endif
