#ifndef BOT_MESSAGEHANDLER_H
#define BOT_MESSAGEHANDLER_H

#include <memory>
#include <vector>

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
        using StringCol = std::vector<std::string>;
        class Sender: public MessageProcessor::Sender
        {
        public:
            virtual void send(const std::string &msg) override;

            const StringCol &getMessages() const
            {
                return messages;
            }

            void reset()
            {
                messages.clear();
            }

        private:
            StringCol messages;
        };

    private:
        gloox::MessageSession &session;
        Sender sender;
        std::unique_ptr<MessageProcessor> messageProcessor;
    };
}

#endif
