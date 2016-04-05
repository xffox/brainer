#include "Bot.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <locale>
#include <codecvt>

#include "xlog/xlog.h"

#include "base/FileConfig.h"

XLOG_SET_LOGGER(xlog::ConsoleLogger)

namespace
{
    struct Config
    {
        std::string jid;
        std::string password;
        std::string resource;
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
        Config result;
        {
            auto jidIter = conf.find(L"jid");
            if(jidIter == conf.end())
                throw std::runtime_error("config: jid missing");
            result.jid = convert.to_bytes(jidIter->second);
        }
        {
            auto passwordIter = conf.find(L"password");
            if(passwordIter == conf.end())
                throw std::runtime_error("config: password missing");
            result.password = convert.to_bytes(passwordIter->second);
        }
        {
            auto resourceIter = conf.find(L"resource");
            if(resourceIter != conf.end())
                result.resource = convert.to_bytes(resourceIter->second);
        }
        return result;
    }
}

int main()
{
    try
    {
        const auto config = readConfig("brainer_bot.conf");
        bot::Bot bot(config.jid, config.password);
        bot.run();
    }
    catch(const std::exception &exc)
    {
        std::cerr<<"error: "<<exc.what()<<std::endl;
        return 1;
    }
    return 0;
}
