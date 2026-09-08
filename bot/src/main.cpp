#include "Bot.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <iterator>
#include <utility>
#include <locale>
#include <codecvt>

#include <CLI/CLI.hpp>
#include <xlog/xlog.hpp>

#include "base/FileConfig.h"

#include "SignalManager.h"

XLOG_DEFINE_CONFIG(xlog::DEFAULT_LOG_LEVEL, xlog::ConsoleLogger, xlog::DefaultFormatter)

namespace
{
    struct Config
    {
        std::optional<std::string> jid;
        std::optional<std::string> password;
        std::optional<std::string> resource;
        std::optional<std::string> room;
        std::optional<std::string> tasksFile;
        std::optional<std::string> host;
    };

    template<typename ConfLeft, typename ConfRight>
    Config join(ConfLeft &&left, ConfRight &&right)
    {
        const auto joinValue = [&](auto Config::* member) -> decltype(auto) {
            return right.*member
                ? std::forward<ConfRight>(right).*member
                : std::forward<ConfLeft>(left).*member;
        };
        return Config {
            joinValue(&Config::jid),
            joinValue(&Config::password),
            joinValue(&Config::resource),
            joinValue(&Config::room),
            joinValue(&Config::tasksFile),
            joinValue(&Config::host),
        };
    }

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
            if(tasksFileIter != conf.end())
            {
                result.tasksFile = convert.to_bytes(tasksFileIter->second);
            }
        }
        {
            auto jidIter = conf.find(L"jid");
            if(jidIter != conf.end())
            {
                result.jid = convert.to_bytes(jidIter->second);
            }
        }
        {
            auto passwordIter = conf.find(L"password");
            if(passwordIter != conf.end())
            {
                result.password = convert.to_bytes(passwordIter->second);
            }
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

int main(const int argc, const char *const *const argv)
{
    CLI::App app;
    std::vector<std::string> configPaths;
    app.add_option("--config", configPaths)->default_val(std::vector<std::string>{"brainer_bot.conf"});
    CLI11_PARSE(app, argc, argv);
    try
    {
        Config config{};
        for (const auto &configPath : configPaths)
        {
            config = join(config, readConfig(configPath));
        }
        if(!config.tasksFile)
        {
            throw std::runtime_error("config: tasks_file missing");
        }
        if(!config.jid)
        {
            throw std::runtime_error("config: jid missing");
        }
        if(!config.password)
        {
            throw std::runtime_error("config: password missing");
        }
        bot::Bot bot(config.tasksFile.value(), config.jid.value(),
            config.password.value(), config.resource.value_or(""),
            config.room.value_or(""), config.host.value_or(""));
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
