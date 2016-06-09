#include "MessageHandler.h"

#include <cassert>
#include <sstream>

#include <gloox/messagesession.h>
#include <gloox/message.h>

#include "base/Nullable.h"
#include "task/HexByteTaskGenerator.h"
#include "parser.h"
#include "strutil.h"
#include "StringRender.h"

namespace bot
{
    MessageHandler::MessageHandler(gloox::MessageSession &session,
        task::TaskProvider &taskProvider)
        :session(session), taskProvider(taskProvider), taskLogic()
    {
    }

    // TODO: measure response time
    void MessageHandler::handleMessage(const gloox::Message& msg,
        gloox::MessageSession*)
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
                            const auto answer = strutil::toCoreString(msg.body());
                            if(taskLogic->validate(answer))
                            {
                                sendValid(*taskLogic, answer);
                                sendTask(*taskLogic);
                            }
                            else
                            {
                                sendInvalid(*taskLogic, answer);
                            }
                        }
                        break;
                    case parser::Input::TYPE_CMD:
                        {
                            if(input->cmd.cmd == "skip")
                            {
                                const auto answer = taskLogic->skip();
                                sendAnswer(answer);
                                sendTask(*taskLogic);
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

    void MessageHandler::sendTask(core::TaskLogic &logic)
    {
        StringRender render;
        logic.describe(render);
        session.send(strutil::fromCoreString(render.text()));
    }

    void MessageHandler::sendInvalid(core::TaskLogic &logic, const core::String &str)
    {
        session.send(strutil::fromCoreString(L"WRONG: " + description(logic) + L" isn't " +  str));
    }

    void MessageHandler::sendValid(core::TaskLogic &logic, const core::String &str)
    {
        session.send(strutil::fromCoreString(L"RIGHT: " + description(logic) + L" is " +  str));
    }

    void MessageHandler::sendAnswer(const core::String &str)
    {
        session.send(strutil::fromCoreString(L"answer is " + str));
    }

    core::String MessageHandler::description(core::TaskLogic &logic)
    {
        StringRender render;
        logic.describe(render);
        return render.text();
    }

    bool MessageHandler::playTasks(const std::string &name)
    {
        std::unique_ptr<core::ITaskGenerator> taskGenerator;
        try
        {
            taskGenerator.reset(taskProvider.create(name).release());
        }
        catch(const std::exception&)
        {
            return false;
        }
        session.send("let's play");
        taskLogic.reset(new core::TaskLogic(std::move(taskGenerator)));
        sendTask(*taskLogic);
        return true;
    }

    void MessageHandler::quitTasks()
    {
        session.send("quit");
        taskLogic.reset();
    }

    void MessageHandler::listTasks()
    {
        std::stringstream ss;
        ss<<"tasks:"<<std::endl;
        const auto tasks = taskProvider.getTasks();
        for(const auto &task : tasks)
            ss<<task<<std::endl;
        session.send(ss.str());
    }

    void MessageHandler::sendNormHelp()
    {
        session.send(".play <game>");
        session.send(".list");
    }

    void MessageHandler::sendPlayHelp()
    {
        session.send("<answer>\n.quit\n.skip");
    }
}
