#include "Bot.h"

#include <memory>
#include <cassert>
#include <stdexcept>

#include <gloox/jid.h>
#include <gloox/client.h>
#include <gloox/jid.h>

#include "xlog/xlog.h"

#include "ConnectionHandler.h"

namespace bot
{
    Bot::Bot(const std::string &tasksFile,
        const std::string &jid, const std::string &password,
        const std::string &resource, const std::string &room)
        :cont(true), taskProvider(new task::TaskProvider(tasksFile)),
        client(new gloox::Client(gloox::JID(jid), password)),
        connectionHandler(new ConnectionHandler(*client, *taskProvider, room))
    {
        client->setResource(resource);
    }

    Bot::~Bot()
    {
        assert(client.get());
        assert(connectionHandler.get());
    }

    void Bot::run()
    {
        assert(client.get());
        xlog::log().info("Bot", "running brainer bot");
        while(cont)
        {
            xlog::log().info("Bot", "connecting");
            if(!client->connect())
                xlog::log().error("Bot", "connection error");
        }
        xlog::log().info("Bot", "brainer bot stopped");
    }

    void Bot::kill()
    {
        assert(client.get());
        cont = false;
        client->disconnect();
    }
}
