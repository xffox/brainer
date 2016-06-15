#ifndef BOT_MESSAGEHANDLER_H
#define BOT_MESSAGEHANDLER_H

#include <memory>
#include <vector>

#include <gloox/messagehandler.h>

#include "MessageProcessor.h"
#include "CollectingSender.h"

namespace task
{
    class TaskProvider;
}

namespace bot
{
    class MessageHandler: public gloox::MessageHandler
    {
    public:
        MessageHandler(gloox::MessageSession &session,
            task::TaskProvider &taskProvider);

    protected:
        virtual void handleMessage(const gloox::Message& msg,
            gloox::MessageSession *session);

    private:
        gloox::MessageSession &session;
        CollectingSender sender;
        std::unique_ptr<MessageProcessor> messageProcessor;
    };
}

#endif
