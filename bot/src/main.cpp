#include "Bot.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <iterator>
#include <locale>
#include <codecvt>

#include <xlog/xlog.hpp>
#include "base/FileConfig.h"

#include "SignalManager.h"

XLOG_DEFINE_CONFIG(xlog::DEFAULT_LOG_LEVEL, xlog::ConsoleLogger, xlog::DefaultFormatter)

namespace
{
    struct Config
    {
        std::string jid;
        std::string password;
        std::string resource;
        std::string room;
        std::string tasksFile;
        std::string host;
    };

    Config readConfig(const std::string &filename)
    {
        using ConfMap = std::unordered_map<std::wstring, std::wstring>;
        base::FileConfig fc(filename);
        base::IConfig::ValuesCollection values;
        try
        {
            values = fc.read();
        }
        catch(const std::exception &exc)
        {
            throw std::runtime_error("failed to read config");
        }
        ConfMap conf(values.begin(), values.end());
        std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
        Config result{};
        {
            auto tasksFileIter = conf.find(L"tasks_file");
            if(tasksFileIter == conf.end())
            {
                throw std::runtime_error("config: tasks_file missing");
            }
            result.tasksFile = convert.to_bytes(tasksFileIter->second);
        }
        {
            auto jidIter = conf.find(L"jid");
            if(jidIter == conf.end())
            {
                throw std::runtime_error("config: jid missing");
            }
            result.jid = convert.to_bytes(jidIter->second);
        }
        {
            auto passwordIter = conf.find(L"password");
            if(passwordIter == conf.end())
            {
                throw std::runtime_error("config: password missing");
            }
            result.password = convert.to_bytes(passwordIter->second);
        }
        {
            auto resourceIter = conf.find(L"resource");
            if(resourceIter != conf.end())
            {
                result.resource = convert.to_bytes(resourceIter->second);
            }
        }
        {
            auto roomIter = conf.find(L"room");
            if(roomIter != conf.end())
            {
                result.room = convert.to_bytes(roomIter->second);
            }
        }
        {
            auto hostIter = conf.find(L"host");
            if(hostIter != std::end(conf))
            {
                result.host = convert.to_bytes(hostIter->second);
            }
        }
        return result;
    }
}

int main()
{
    try
    {
        const auto config = readConfig("brainer_bot.conf");
        bot::Bot bot(config.tasksFile, config.jid, config.password,
            config.resource, config.room, config.host);
        auto term = [&bot](){bot.kill();};
        try
        {
            bot::SignalManager::getInstance().setHandler(
                bot::SignalManager::SIGNAL_INT, term);
            bot::SignalManager::getInstance().setHandler(
                bot::SignalManager::SIGNAL_TERM, term);
            bot.run();
        }
        catch(const std::exception&)
        {
            try
            {
                bot::SignalManager::getInstance().clearHandler(
                    bot::SignalManager::SIGNAL_INT);
            }
            catch(const std::exception&)
            {}
            try
            {
                bot::SignalManager::getInstance().clearHandler(
                    bot::SignalManager::SIGNAL_TERM);
            }
            catch(const std::exception&)
            {}
            throw;
        }
    }
    catch(const std::exception &exc)
    {
        xlog::log().error("main", "error: %s", exc.what());
        return 1;
    }
    return 0;
}
