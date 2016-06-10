#include "MessageHandler.h"

#include <cassert>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <cstddef>

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
                            const auto descr = description(*taskLogic);
                            if(taskLogic->validate(answer))
                            {
                                sendValid(descr, answer, taskLogic->getStats());
                                sendTask(*taskLogic);
                            }
                            else
                            {
                                sendInvalid(descr, answer, taskLogic->getStats());
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

    void MessageHandler::sendInvalid(const core::String &descr, const core::String &str,
        const core::TaskLogic::StatsCol &stats)
    {
        session.send(strutil::fromCoreString(L"WRONG: " + descr + L" isn't " +  str));
    }

    void MessageHandler::sendValid(const core::String &descr, const core::String &str,
        const core::TaskLogic::StatsCol &stats)
    {
        std::wstringstream stream;
        if(stats.empty())
            throw std::runtime_error("stats list is empty on valid result");
        const auto elapsedUs = stats.back().timeUs;
        stream<<L"RIGHT: "<<descr<<L" is "<<str
            <<" ("<<std::setprecision(2)<<std::fixed
            <<static_cast<double>(elapsedUs)/1000000.0<<"s)";
        session.send(strutil::fromCoreString(stream.str()));
    }

    void MessageHandler::sendAnswer(const core::String &str)
    {
        session.send(strutil::fromCoreString(L"answer is " + str));
    }

    void MessageHandler::sendStats(const core::TaskLogic::StatsCol &stats)
    {
        if(stats.empty())
            return;
        std::wstringstream stream;
        double averageTime = 0.0;
        std::size_t answeredCount = 0;
        std::size_t wrongCount = 0;
        for(const auto &s : stats)
        {
            averageTime += s.timeUs;
            if(s.answered)
                answeredCount += 1;
            wrongCount += s.tries;
        }
        averageTime /= stats.size();
        stream<<"played: "<<stats.size()<<" tasks"<<std::endl
            <<"answered: "<<answeredCount<<" tasks"<<std::endl
            <<"wrong: "<<wrongCount<<" answers"<<std::endl
            <<"average time: "<<std::setprecision(2)<<std::fixed
                <<averageTime/1000000.0<<"s";
        session.send(strutil::fromCoreString(stream.str()));
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
        assert(taskLogic.get());
        sendStats(taskLogic->getStats());
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
