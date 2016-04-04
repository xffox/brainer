#include "Bot.h"

#include <memory>
#include <cassert>
#include <stdexcept>

#include <gloox/jid.h>
#include <gloox/client.h>
#include <gloox/jid.h>

#include "ConnectionHandler.h"
#include "MessageSessionHandler.h"

namespace bot
{
    Bot::Bot(const std::string &jid, const std::string &password)
        :client(new gloox::Client(gloox::JID(jid), password)),
        connectionHandler(new ConnectionHandler()),
        messageSessionHandler(new MessageSessionHandler())
    {
        client->registerConnectionListener(connectionHandler.get());
        client->registerMessageSessionHandler(messageSessionHandler.get());
    }
    
    Bot::~Bot()
    {}

    void Bot::run()
    {
        assert(client.get());
        if(!client->connect())
            throw std::runtime_error("connection error");
    }
}
