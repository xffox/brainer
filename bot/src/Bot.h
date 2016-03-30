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
        Bot(const std::string &jid, const std::string &password);
        ~Bot();

        void run();
        
    private:
        std::auto_ptr<gloox::Client> client;
        std::auto_ptr<ConnectionHandler> connectionHandler;
        std::auto_ptr<MessageSessionHandler> messageSessionHandler;
    };
}

#endif
