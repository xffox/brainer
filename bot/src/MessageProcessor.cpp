#include "MessageProcessor.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include <cstddef>

#include "task/TaskProvider.h"
#include "core/TaskLogic.h"
#include "core/ITaskGenerator.h"

#include "parser.h"
#include "strutil.h"
#include "StringRender.h"

namespace bot
{
    MessageProcessor::MessageProcessor(Sender &sender,
        task::TaskProvider &taskProvider)
        :sender(sender), taskProvider(taskProvider)
    {
    }

    MessageProcessor::~MessageProcessor()
    {}

    void MessageProcessor::receive(const std::string &from,
        const std::string &msg)
    {
        const auto &body = msg;
        if(body.empty())
            return;
        const auto input = parser::parse(body);
        if(!input.isNull())
        {
            switch(input->type)
            {
            case parser::Input::TYPE_MSG:
                processMessage(from, input->msg);
                break;
            case parser::Input::TYPE_CMD:
                processCommand(from, input->cmd);
                break;
            default:
                assert(false);
                break;
            }
        }
        else
        {
            processHelpCmd(from, StringList());
        }
    }

    void MessageProcessor::processCommand(const std::string &from, const Command &command)
    {
        if(command.cmd == "skip")
        {
            processSkipCmd(from, command.args);
        }
        else if(command.cmd == "play")
        {
            processPlayCmd(from, command.args);
        }
        else if(command.cmd == "quit")
        {
            processQuitCmd(from, command.args);
        }
        else if(command.cmd == "help")
        {
            processHelpCmd(from, command.args);
        }
        else if(command.cmd == "list")
        {
            processListCmd(from, command.args);
        }
        else
        {
            sendNormHelp();
        }
    }

    void MessageProcessor::processMessage(const std::string &from, const std::string &message)
    {
        processAnswer(from, message);
    }

    void MessageProcessor::processListCmd(const std::string&, const StringList&)
    {
        sendTaskList();
    }

    void MessageProcessor::processHelpCmd(const std::string&, const StringList&)
    {
        if(taskLogic.get())
            sendPlayHelp();
        else
            sendNormHelp();
    }

    void MessageProcessor::processPlayCmd(const std::string &from, const StringList &args)
    {
        if(args.size() != 1)
        {
            sendTaskList();
            return;
        }
        const auto &name = args[0];
        if(!play(name))
        {
            sendTaskList();
            return;
        }
        send("let's play");
        sendTask(*getTaskLogic());
    }

    void MessageProcessor::processQuitCmd(const std::string &from, const StringList &args)
    {
        if(taskLogic.get())
        {
            sendStats(getTaskLogic()->getStats());
            send("quit");
            quit();
        }
        else
        {
            sendNormHelp();
        }
    }

    void MessageProcessor::processSkipCmd(const std::string &from, const StringList &args)
    {
        if(taskLogic.get())
        {
            const auto answer = skip();
            sendAnswer(answer);
            sendTask(*taskLogic);
        }
        else
        {
            sendNormHelp();
        }
    }

    base::Nullable<MessageProcessor::Validity> MessageProcessor::processAnswer(const std::string &from, const std::string &answer)
    {
        if(taskLogic.get())
        {
            const auto ans = strutil::toCoreString(answer);
            const auto descr = description(*taskLogic);
            if(answerTask(ans))
            {
                sendValid(from, descr, ans, taskLogic->getStats());
                sendTask(*taskLogic);
                return base::Nullable<Validity>(VALID);
            }
            else
            {
                sendInvalid(from, descr, ans);
                return base::Nullable<Validity>(INVALID);
            }
        }
        else
        {
            send("no game is played now");
            sendNormHelp();
            return base::Nullable<Validity>();
        }
    }

    void MessageProcessor::send(const std::string &msg)
    {
        sender.send(msg);
    }

    void MessageProcessor::sendTaskList()
    {
        std::stringstream ss;
        ss<<"tasks:"<<std::endl;
        const auto tasks = taskProvider.getTasks();
        for(const auto &task : tasks)
            ss<<task<<std::endl;
        send(ss.str());
    }

    void MessageProcessor::sendNormHelp()
    {
        std::stringstream ss;
        ss<<".play <game>"<<std::endl
            <<".list";
        send(ss.str());
    }

    void MessageProcessor::sendPlayHelp()
    {
        std::stringstream ss;
        ss<<"<answer>"<<std::endl
            <<".quit"<<std::endl
            <<".skip";
        send(ss.str());
    }

    void MessageProcessor::sendTask(core::TaskLogic &logic)
    {
        StringRender render;
        logic.describe(render);
        send(strutil::fromCoreString(render.text()));
    }

    void MessageProcessor::sendInvalid(const std::string &from,
        const core::String &descr, const core::String &str)
    {
        send(strutil::fromCoreString(L"WRONG: " + descr + L" isn't " +  str));
    }

    void MessageProcessor::sendValid(const std::string &from, const core::String &descr,
        const core::String &str, const core::TaskLogic::StatsCol &stats)
    {
        std::wstringstream stream;
        if(stats.empty())
            throw std::runtime_error("stats list is empty on valid result");
        const auto elapsedUs = stats.back().timeUs;
        stream<<L"RIGHT: "<<descr<<L" is "<<str
            <<" ("<<std::setprecision(2)<<std::fixed
            <<static_cast<double>(elapsedUs)/1000000.0<<"s)";
        send(strutil::fromCoreString(stream.str()));
    }

    void MessageProcessor::sendAnswer(const core::String &str)
    {
        send(strutil::fromCoreString(L"answer is " + str));
    }

    void MessageProcessor::sendStats(const core::TaskLogic::StatsCol &stats)
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
        stream<<"stats:"<<std::endl;
        stream<<"played: "<<stats.size()<<" tasks"<<std::endl
            <<"answered: "<<answeredCount<<" tasks"<<std::endl
            <<"wrong: "<<wrongCount<<" answers"<<std::endl
            <<"average time: "<<std::setprecision(2)<<std::fixed
                <<averageTime/1000000.0<<"s";
        send(strutil::fromCoreString(stream.str()));
    }

    core::String MessageProcessor::description(core::TaskLogic &logic)
    {
        StringRender render;
        logic.describe(render);
        return render.text();
    }

    core::TaskLogic *MessageProcessor::getTaskLogic() const
    {
        return taskLogic.get();
    }

    bool MessageProcessor::play(const std::string &name)
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
        taskLogic.reset(new core::TaskLogic(std::move(taskGenerator)));
        return true;
    }

    bool MessageProcessor::answerTask(const core::String &answer)
    {
        if(taskLogic.get())
            return taskLogic->validate(answer);
        else
            return false;
    }

    void MessageProcessor::quit()
    {
        taskLogic.reset();
    }

    core::String MessageProcessor::skip()
    {
        if(taskLogic.get())
        {
            return taskLogic->skip();
        }
        else
        {
            return core::String();
        }
    }
}
