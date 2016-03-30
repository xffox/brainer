#include "MessageSessionHandler.h"

#include <exception>
#include <cassert>

namespace bot
{
    void MessageSessionHandler::handleMessageSession(
        gloox::MessageSession *session)
    {
        assert(session);
        std::shared_ptr<MessageHandler> handler(new MessageHandler(session));
        session->registerMessageHandler(handler.get());
        try
        {
            messageHandlers.push_back(handler);
        }
        catch(const std::exception&)
        {
            session->removeMessageHandler();
        }
    }
}
