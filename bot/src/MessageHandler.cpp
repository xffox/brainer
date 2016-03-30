#include "MessageHandler.h"

#include <cassert>
#include <codecvt>
#include <locale>
#include <sstream>

#include <gloox/messagesession.h>
#include <gloox/message.h>

#include "base/Nullable.h"
#include "task/HexByteTaskGenerator.h"
#include "parser.h"

namespace bot
{
    namespace
    {
        std::string fromCoreString(const core::String &str)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
            return convert.to_bytes(str);
        }

        core::String toCoreString(const std::string &str)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
            return convert.from_bytes(str);
        }
    }

    MessageHandler::MessageHandler(gloox::MessageSession *session)
        :session(session), taskProvider(new task::TaskProvider()), taskLogic()
    {
    }

    // TODO: measure response time
    void MessageHandler::handleMessage(const gloox::Message& msg,
        gloox::MessageSession *session)
    {
        if(msg.subtype() == gloox::Message::Chat)
        {
            const auto &body = msg.body();
            if(body.empty())
                return;
            const auto input = parser::parse(body);
            if(taskLogic.get() == nullptr)
            {
                if(input.isNull() || input->type == parser::Input::TYPE_MSG)
                {
                    sendNormHelp();
                }
                else
                {
                    assert(input->type == parser::Input::TYPE_CMD);
                    if(input->cmd.cmd == "play")
                    {
                        if(input->cmd.args.size() != 1)
                        {
                            sendNormHelp();
                        }
                        else
                        {
                            if(!playTasks(input->cmd.args[0]))
                                listTasks();
                        }
                    }
                    else if(input->cmd.cmd == "list")
                    {
                        listTasks();
                    }
                    else
                    {
                        sendNormHelp();
                    }
                }
            }
            else
            {
                if(input.isNull())
                {
                    sendPlayHelp();
                }
                else
                {
                    switch(input->type)
                    {
                    case parser::Input::TYPE_MSG:
                        {
                            taskLogic->validate(toCoreString(msg.body()));
                        }
                        break;
                    case parser::Input::TYPE_CMD:
                        {
                            if(input->cmd.cmd == "skip")
                            {
                                taskLogic->skip();
                            }
                            else if(input->cmd.cmd == "quit")
                            {
                                quitTasks();
                            }
                            else
                            {
                                sendPlayHelp();
                            }
                        }
                        break;
                    default:
                        assert(false);
                        break;
                    }
                }
            }
        }
    }

    void MessageHandler::showTask(const core::String &str)
    {
        assert(session);
        session->send(fromCoreString(str));
    }

    void MessageHandler::showInvalid(const core::String &str)
    {
        assert(session);
        session->send(fromCoreString(str + L" is WRONG"));
    }

    void MessageHandler::showAnswer(const core::String &str)
    {
        assert(session);
        session->send(fromCoreString(L"answer is " + str));
    }

    bool MessageHandler::playTasks(const std::string &name)
    {
        assert(session);
        assert(taskProvider.get());
        std::auto_ptr<core::ITaskGenerator> taskGenerator;
        try
        {
            taskGenerator.reset(taskProvider->create(name).release());
        }
        catch(const std::exception&)
        {
            return false;
        }
        session->send("let's play");
        taskLogic.reset(new core::TaskLogic(taskGenerator, *this));
        return true;
    }

    void MessageHandler::quitTasks()
    {
        assert(session);
        session->send("quit");
        taskLogic.reset();
    }

    void MessageHandler::listTasks()
    {
        assert(session);
        assert(taskProvider.get());
        std::stringstream ss;
        ss<<"tasks:"<<std::endl;
        const auto tasks = taskProvider->getTasks();
        for(const auto &task : tasks)
            ss<<task<<std::endl;
        session->send(ss.str());
    }

    void MessageHandler::sendNormHelp()
    {
        assert(session);
        session->send(".play <game>");
        session->send(".list");
    }

    void MessageHandler::sendPlayHelp()
    {
        assert(session);
        session->send("<answer>\n.quit\n.skip");
    }
}
