#include "Bot.h"

#include <memory>
#include <cassert>
#include <chrono>
#include <thread>
#include <mutex>
#include <stdexcept>

#include <gloox/jid.h>
#include <gloox/client.h>
#include <gloox/jid.h>
#include <gloox/gloox.h>
#include <gloox/eventhandler.h>

#include <xlog/xlog.hpp>

#include <base/randomizer.hpp>
#include "ConnectionHandler.h"
#include "LogHandler.h"

namespace bot
{
    namespace
    {
        using Clock = std::chrono::steady_clock;

        constexpr auto RECV_TIMEOUT =
            std::chrono::seconds(30);
        constexpr auto PING_PERIOD =
            std::chrono::seconds(30);
        constexpr auto PING_TIMEOUT = PING_PERIOD*2;
        constexpr auto RECONNECT_TIME =
            std::chrono::seconds(10);
    }

    class Bot::PingEventHandler: public gloox::EventHandler
    {
    public:
        void reset()
        {
            lastSuccessTime_ = Clock::now();
        }

        void handleEvent(const gloox::Event &event) override
        {
            if(event.eventType() == gloox::Event::PingPong)
            {
                lastSuccessTime_ = Clock::now();
            }
        }

        const auto &lastSuccessTime() const
        {
            return lastSuccessTime_;
        }

    private:
        Clock::time_point lastSuccessTime_{};
    };

    class Bot::BotClient
    {
    public:
        BotClient(const std::string &jid, const std::string &password,
            const std::string &resource, const std::string &room,
            const std::string &host,
            task::TaskProvider &taskProvider,
            LogHandler &logHandler)
            :client_(new gloox::Client(gloox::JID(jid), password)),
            connectionHandler(
                new ConnectionHandler(*client_, taskProvider, room))
        {
            client_->logInstance().registerLogHandler(
                gloox::LogLevel::LogLevelWarning, gloox::LogArea::LogAreaAll,
                &logHandler);
            client_->logInstance().registerLogHandler(
                gloox::LogLevel::LogLevelDebug, gloox::LogArea::LogAreaAll,
                &logHandler);
            client_->logInstance().registerLogHandler(
                gloox::LogLevel::LogLevelError, gloox::LogArea::LogAreaAll,
                &logHandler);
            if(!host.empty())
                client_->setServer(host);
            client_->setResource(resource);
            client_->setStreamManagement();
        }

        gloox::Client &client()
        {
            return *client_;
        }

    private:
        std::unique_ptr<gloox::Client> client_;
        std::unique_ptr<ConnectionHandler> connectionHandler;
    };

    Bot::Bot(const std::string &tasksFile,
        const std::string &jid, const std::string &password,
        const std::string &resource, const std::string &room,
        const std::string &host)
        :jid(jid), password(password), resource(resource),
        host(host), room(room),
        cont(true), taskProvider(
            new task::TaskProvider(base::Randomizer(), tasksFile)),
        logHandler(new LogHandler()),
        pingHandler(new PingEventHandler()),
        client{}, clientMutex{}
    {}

    Bot::~Bot()
    {}

    void Bot::run()
    {
        xlog::log().info("Bot", "running brainer bot");
        while(initClient())
        {
            xlog::log().info("Bot", "connecting: %s",
                client->client().jid().full().c_str());
            if(client->client().connect(false))
            {
                xlog::log().info("Bot", "connection established: %s",
                    client->client().jid().full().c_str());
                auto lastPingTime = Clock::now();
                pingHandler->reset();
                while(true)
                {
                    const auto r = client->client().recv(
                        std::chrono::duration_cast<std::chrono::microseconds>(
                            RECV_TIMEOUT).count());
                    if(r == gloox::ConnUserDisconnected)
                    {
                        break;
                    }
                    if(r != gloox::ConnNoError)
                    {
//                        client->disconnect();
                        break;
                    }
                    const auto curTime = Clock::now();
                    if(curTime - pingHandler->lastSuccessTime() >=
                        PING_TIMEOUT)
                    {
                        xlog::log().info("Bot", "connection timeout");
                        client->client().disconnect();
                        continue;
                    }
                    if(curTime - lastPingTime >= PING_PERIOD)
                    {
                        client->client().xmppPing(
                            client->client().jid(), pingHandler.get());
                        lastPingTime = curTime;
                    }
                }
            }
            if(cont)
            {
                xlog::log().error("Bot", "reconnecting on connection error");
                std::this_thread::sleep_for(RECONNECT_TIME);
            }
        }
        xlog::log().info("Bot", "brainer bot stopped");
    }

    void Bot::kill()
    {
        std::lock_guard<std::mutex> lock(clientMutex);
        cont = false;
        if(client)
        {
            client->client().disconnect();
        }
    }

    bool Bot::initClient()
    {
        std::lock_guard<std::mutex> lock(clientMutex);
        if(cont)
        {
            client.reset(new BotClient(jid, password, resource, host, room,
                    *taskProvider, *logHandler));
        }
        return cont;
    }
}
