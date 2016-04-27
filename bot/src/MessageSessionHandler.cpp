#include "MessageSessionHandler.h"

#include <exception>
#include <cassert>

namespace bot
{
    MessageSessionHandler::MessageSessionHandler(task::TaskProvider &taskProvider)
        :taskProvider(taskProvider)
    {}

    void MessageSessionHandler::handleMessageSession(
        gloox::MessageSession *session)
    {
        assert(session);
        std::shared_ptr<MessageHandler> handler(new MessageHandler(*session,
                taskProvider));
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
