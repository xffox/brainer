#include "MessageHandler.h"

#include <cassert>
#include <sstream>

#include <gloox/messagesession.h>
#include <gloox/message.h>

#include "MessageProcessor.h"

namespace bot
{
    MessageHandler::MessageHandler(gloox::MessageSession &session,
        task::TaskProvider &taskProvider)
        :session(session), sender(),
        messageProcessor(new MessageProcessor(sender, taskProvider))
    {}

    // TODO: measure response time
    void MessageHandler::handleMessage(const gloox::Message& msg,
        gloox::MessageSession*)
    {
        assert(messageProcessor.get());
        if(msg.subtype() == gloox::Message::Chat)
        {
            messageProcessor->receive(msg.from().username(), msg.body());
            const auto &msgs = sender.getMessages();
            std::stringstream stream;
            for(CollectingSender::StringCol::size_type i = 0; i < msgs.size(); ++i)
            {
                stream<<msgs[i];
                if(i+1 < msgs.size())
                    stream<<std::endl;
            }
            session.send(stream.str());
            sender.reset();
        }
    }
}
