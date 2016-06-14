#ifndef BOT_MESSAGEHANDLER_H
#define BOT_MESSAGEHANDLER_H

#include <memory>

#include <gloox/messagehandler.h>

#include "MessageProcessor.h"

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
        class Sender: public MessageProcessor::Sender
        {
        public:
            Sender(gloox::MessageSession &session)
                :session(session)
            {}

            virtual void send(const std::string &msg) override;

        private:
            gloox::MessageSession &session;
        };

    private:
        gloox::MessageSession &session;
        Sender sender;
        std::unique_ptr<MessageProcessor> messageProcessor;
    };
}

#endif
