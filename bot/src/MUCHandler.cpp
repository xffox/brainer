#include "MUCHandler.h"

#include <cassert>
#include <sstream>

#include <gloox/message.h>

#include "xlog/xlog.h"

#include "core/ITaskGenerator.h"

#include "strutil.h"
#include "parser.h"
#include "StringRender.h"

namespace bot
{
    MUCHandler::MUCHandler(gloox::MUCRoom &room,
        task::TaskProvider &taskProvider)
        :room(room), taskProvider(taskProvider), taskLogic()
    {}

    void MUCHandler::handleMUCConfigList(gloox::MUCRoom*, const gloox::MUCListItemList&, gloox::MUCOperation)
    {
    }

    void MUCHandler::handleMUCConfigForm(gloox::MUCRoom*, const gloox::DataForm&)
    {
    }

    void MUCHandler::handleMUCConfigResult(gloox::MUCRoom*, bool, gloox::MUCOperation)
    {
    }

    void MUCHandler::handleMUCRequest(gloox::MUCRoom*, const gloox::DataForm&)
    {
    }

    void MUCHandler::handleMUCParticipantPresence(gloox::MUCRoom*,
        gloox::MUCRoomParticipant, const gloox::Presence&)
    {
    }

    void MUCHandler::handleMUCMessage(gloox::MUCRoom*,
        const gloox::Message &msg, bool priv)
    {
        xlog::log().info("MUCHandler", "message: f=%s n=%s", msg.from().full().c_str(),
            room.nick().c_str());
        if(priv || msg.from().resource() == room.nick())
            return;
        if(msg.subtype() == gloox::Message::Groupchat)
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
                                sendValid(msg.from().resource(), answer);
                                sendTask(*taskLogic);
                            }
                            else
                            {
                                sendInvalid(msg.from().resource(), answer);
                            }
                        }
                        break;
                    case parser::Input::TYPE_CMD:
                        {
                            if(input->cmd.cmd == "skip")
                            {
                                taskLogic->skip();
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

    bool MUCHandler::handleMUCRoomCreation(gloox::MUCRoom*)
    {
        return true;
    }

    void MUCHandler::handleMUCSubject(gloox::MUCRoom*, const std::string&, const std::string&)
    {
    }

    void MUCHandler::handleMUCInviteDecline(gloox::MUCRoom*, const gloox::JID&, const std::string&)
    {
    }

    void MUCHandler::handleMUCError(gloox::MUCRoom *room,
        gloox::StanzaError error)
    {
        xlog::log().error("MUCHandler", "muc error: %d", error);
    }

    void MUCHandler::handleMUCInfo(gloox::MUCRoom*, int, const std::string&, const gloox::DataForm*)
    {
    }

    void MUCHandler::handleMUCItems(gloox::MUCRoom*, const gloox::Disco::ItemList&)
    {
    }

    void MUCHandler::sendTask(core::TaskLogic &logic)
    {
        StringRender render;
        logic.describe(render);
        room.send(strutil::fromCoreString(render.text()));
    }

    void MUCHandler::sendValid(const std::string &nick, const core::String &str)
    {
        room.send(nick + ", " + strutil::fromCoreString(str) + " is RIGHT");
    }

    void MUCHandler::sendInvalid(const std::string &nick, const core::String &str)
    {
        room.send(nick + ", " + strutil::fromCoreString(str) + " is WRONG");
    }

    void MUCHandler::sendAnswer(const core::String &str)
    {
        room.send(strutil::fromCoreString(L"answer is " + str));
    }

    bool MUCHandler::playTasks(const std::string &name)
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
        room.send("let's play");
        taskLogic.reset(new core::TaskLogic(std::move(taskGenerator)));
        sendTask(*taskLogic);
        return true;
    }

    void MUCHandler::quitTasks()
    {
        room.send("quit");
        taskLogic.reset();
    }

    void MUCHandler::listTasks()
    {
        std::stringstream ss;
        ss<<"tasks:"<<std::endl;
        const auto tasks = taskProvider.getTasks();
        for(const auto &task : tasks)
            ss<<task<<std::endl;
        room.send(ss.str());
    }

    void MUCHandler::sendNormHelp()
    {
        std::stringstream ss;
        ss<<".play <game>"<<std::endl
            <<".list";
        room.send(ss.str());
    }

    void MUCHandler::sendPlayHelp()
    {
        std::stringstream ss;
        ss<<"<answer>"<<std::endl
            <<"quit"<<std::endl
            <<"skip";
        room.send(ss.str());
    }
}
