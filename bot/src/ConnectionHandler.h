#ifndef BOT_CONNECTIONHANDLER_H
#define BOT_CONNECTIONHANDLER_H

#include <memory>
#include <string>
#include <vector>

#include <gloox/connectionlistener.h>
#include <gloox/mucroom.h>
#include <gloox/client.h>
#include <gloox/messagesessionhandler.h>
#include <gloox/jid.h>

#include "task/TaskProvider.h"
#include "MessageHandler.h"

namespace bot
{
    class MUCHandler;
    class RosterManager;

    class ConnectionHandler: public gloox::ConnectionListener,
        public gloox::MessageSessionHandler
    {
    public:
        ConnectionHandler(gloox::Client &client,
            task::TaskProvider &taskProvider,
            const std::string &roomJid = std::string());
        virtual ~ConnectionHandler();

    protected:
        virtual void onConnect() override;
        virtual void onDisconnect(gloox::ConnectionError e) override;
        virtual bool onTLSConnect(const gloox::CertInfo& info) override;
        virtual void onResourceBindError(const gloox::Error *error) override;
        virtual void onSessionCreateError(const gloox::Error *error) override;
        virtual void handleMessageSession(gloox::MessageSession *session) override;

    private:
        using MessageHandlerCollection =
            std::vector<std::shared_ptr<MessageHandler>>;

    private:
        void joinRoom();
        void leaveRoom();

    private:
        std::string roomJid;
        gloox::Client &client;
        task::TaskProvider &taskProvider;

        std::unique_ptr<MUCHandler> mucHandler;
        std::unique_ptr<RosterManager> rosterManager;
        MessageHandlerCollection messageHandlers;
    };
}

#endif
