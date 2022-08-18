#include "MessageHandler.h"

#include <cassert>
#include <sstream>

#include <gloox/messagesession.h>
#include <gloox/message.h>

#include <xlog/xlog.hpp>
#include "MessageProcessor.h"
#include "RosterManager.hpp"

namespace bot
{
    MessageHandler::MessageHandler(gloox::Client &client, gloox::MessageSession *session,
        task::TaskProvider &taskProvider, const RosterManager &rosterManager)
        :client(client), rosterManager(rosterManager),
        session(session), sender(),
        messageProcessor(new MessageProcessor(sender, taskProvider))
    {
        session->registerMessageHandler(this);
        xlog::log().info("MessageHandler", "message session: '%s'",
            session->target().bare().c_str());
    }

    MessageHandler::~MessageHandler()
    {
        session->removeMessageHandler();
        // according to the documentation this shouldn't be called
        // in the initial handler, probably needs to be handled properly
        client.disposeMessageSession(session);
    }

    // TODO: measure response time
    void MessageHandler::handleMessage(const gloox::Message& msg,
        gloox::MessageSession*)
    {
        assert(messageProcessor.get());
        if(!rosterManager.isKnown(msg.from()))
        {
            xlog::log().info("MessageHandler",
                "ignoring unauthorized message: '%s'",
                msg.from().bare().c_str());
            return;
        }
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
            session->send(stream.str());
            sender.reset();
        }
    }
}
