#include "Bot.h"

#include <memory>
#include <cassert>
#include <stdexcept>

#include <gloox/jid.h>
#include <gloox/client.h>
#include <gloox/jid.h>

#include "xlog/xlog.h"

#include "ConnectionHandler.h"
#include "MessageSessionHandler.h"

namespace bot
{
    Bot::Bot(const std::string &jid, const std::string &password,
        const std::string &resource)
        :client(new gloox::Client(gloox::JID(jid), password)),
        connectionHandler(new ConnectionHandler(*client)),
        messageSessionHandler(new MessageSessionHandler())
    {
        client->setResource(resource);
        client->registerConnectionListener(connectionHandler.get());
        client->registerMessageSessionHandler(messageSessionHandler.get());
    }
    
    Bot::~Bot()
    {
        assert(client.get());
        assert(connectionHandler.get());
        client->removeConnectionListener(connectionHandler.get());
    }

    void Bot::run()
    {
        assert(client.get());
        xlog::log().info("Bot", "running brainer bot");
        if(!client->connect())
            throw std::runtime_error("connection error");
        xlog::log().info("Bot", "brainer bot stopped");
    }
}
