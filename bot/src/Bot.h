#ifndef BOT_BOT_H
#define BOT_BOT_H

#include <memory>
#include <mutex>
#include <string>

#include <gloox/client.h>

#include "task/TaskProvider.h"

namespace bot
{
    class ConnectionHandler;
    class LogHandler;

    class Bot
    {
    public:
        Bot(const std::string &tasksFile,
            const std::string &jid, const std::string &password,
            const std::string &resource = "", const std::string &room = "",
            const std::string &host = "");
        ~Bot();

        void run();
        void kill();

    private:
        class PingEventHandler;
        class BotClient;

    private:
        bool initClient();

    private:
        std::string jid;
        std::string password;
        std::string resource;
        std::string host;
        std::string room;
        // TODO: atomic weak
        volatile bool cont;
        std::unique_ptr<task::TaskProvider> taskProvider;
        std::unique_ptr<LogHandler> logHandler;
        std::unique_ptr<PingEventHandler> pingHandler;
        std::unique_ptr<BotClient> client;
        std::mutex clientMutex;
    };
}

#endif
