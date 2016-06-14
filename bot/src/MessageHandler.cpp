#include "MessageHandler.h"

#include <cassert>

#include <gloox/messagesession.h>
#include <gloox/message.h>

#include "MessageProcessor.h"

namespace bot
{
    MessageHandler::MessageHandler(gloox::MessageSession &session,
        task::TaskProvider &taskProvider)
        :session(session), sender(session),
        messageProcessor(new MessageProcessor(sender, taskProvider))
    {}

    // TODO: measure response time
    void MessageHandler::handleMessage(const gloox::Message& msg,
        gloox::MessageSession*)
    {
        assert(messageProcessor.get());
        if(msg.subtype() == gloox::Message::Chat)
        {
            messageProcessor->receive(msg.from().full(), msg.body());
        }
    }

    void MessageHandler::Sender::send(const std::string &msg)
    {
        session.send(msg);
    }
}
