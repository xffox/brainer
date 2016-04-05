#ifndef BOT_BOT_H
#define BOT_BOT_H

#include <memory>
#include <string>

#include <gloox/client.h>

namespace bot
{
    class ConnectionHandler;
    class MessageSessionHandler;

    class Bot
    {
    public:
        Bot(const std::string &jid, const std::string &password,
            const std::string &resources = "");
        ~Bot();

        void run();
        
    private:
        std::unique_ptr<gloox::Client> client;
        std::unique_ptr<ConnectionHandler> connectionHandler;
        std::unique_ptr<MessageSessionHandler> messageSessionHandler;
    };
}

#endif
