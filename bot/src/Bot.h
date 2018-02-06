#ifndef BOT_BOT_H
#define BOT_BOT_H

#include <memory>
#include <string>

#include <gloox/client.h>

#include "task/TaskProvider.h"

namespace bot
{
    class ConnectionHandler;

    class Bot
    {
    public:
        Bot(const std::string &tasksFile,
            const std::string &jid, const std::string &password,
            const std::string &resources = "", const std::string &room = "");
        ~Bot();

        void run();
        void kill();
    private:
        volatile bool cont;
        std::unique_ptr<task::TaskProvider> taskProvider;
        std::unique_ptr<gloox::Client> client;
        std::unique_ptr<ConnectionHandler> connectionHandler;
    };
}

#endif
