#ifndef BOT_MESSAGEHANDLER_H
#define BOT_MESSAGEHANDLER_H

#include <memory>
#include <vector>

#include <gloox/messagehandler.h>
#include <gloox/client.h>

#include "MessageProcessor.h"
#include "CollectingSender.h"

namespace task
{
    class TaskProvider;
}

namespace bot
{
    class RosterManager;

    class MessageHandler: public gloox::MessageHandler
    {
    public:
        MessageHandler(gloox::Client &client, gloox::MessageSession *session,
            task::TaskProvider &taskProvider,
            const RosterManager &rosterManager);
        ~MessageHandler();

        MessageHandler(const MessageHandler&) = delete;
        MessageHandler &operator=(const MessageHandler&) = delete;

    protected:
        virtual void handleMessage(const gloox::Message& msg,
            gloox::MessageSession *session);

    private:
        gloox::Client &client;
        const RosterManager &rosterManager;
        gloox::MessageSession *session;
        CollectingSender sender;
        std::unique_ptr<MessageProcessor> messageProcessor;
    };
}

#endif
