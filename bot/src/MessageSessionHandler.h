#ifndef BOT_MESSAGESESSIONHANDLER_H
#define BOT_MESSAGESESSIONHANDLER_H

#include <memory>
#include <vector>

#include <gloox/messagesessionhandler.h>

#include "task/TaskProvider.h"
#include "MessageHandler.h"

namespace bot
{
    class MessageSessionHandler: public gloox::MessageSessionHandler
    {
    public:
        MessageSessionHandler(task::TaskProvider &taskProvider);

    protected:
        virtual void handleMessageSession(gloox::MessageSession *session);

    private:
        using MessageHandlerCollection =
            std::vector<std::shared_ptr<MessageHandler>>;

    private:
        task::TaskProvider &taskProvider;

        MessageHandlerCollection messageHandlers;
    };
}

#endif
