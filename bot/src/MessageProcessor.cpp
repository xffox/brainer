#include "MessageProcessor.h"

#include <cassert>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <cstddef>
#include <utility>

#include "task/TaskProvider.h"
#include "core/TaskLogic.h"
#include "core/ITaskGenerator.h"

#include "parser.h"
#include "strutil.h"
#include "StringRender.h"

namespace bot
{
    namespace
    {
        constexpr double fractionalSeconds(std::chrono::microseconds time)
        {
            constexpr double MULTIPLIER =
                std::chrono::duration_cast<decltype(time)>(
                    std::chrono::seconds(1)).count();
            return static_cast<double>(time.count())/MULTIPLIER;
        }
    }

    MessageProcessor::MessageProcessor(Sender &sender,
        task::TaskProvider &taskProvider)
        :sender(sender), taskProvider(taskProvider), taskLogic{}
    {}

    MessageProcessor::~MessageProcessor() = default;

    void MessageProcessor::receive(const std::string &from,
        const std::string &msg)
    {
        const auto &body = msg;
        if(body.empty())
        {
            return;
        }
        const auto input = parser::parse(body);
        if(input)
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
        else if(command.cmd == "hint")
        {
            processHintCmd(from, command.args);
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
        if(taskLogic)
        {
            sendPlayHelp();
        }
        else
        {
            sendNormHelp();
        }
    }

    void MessageProcessor::processPlayCmd(const std::string&, const StringList &args)
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

    void MessageProcessor::processQuitCmd(const std::string&, const StringList&)
    {
        if(taskLogic)
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

    void MessageProcessor::processSkipCmd(const std::string&, const StringList&)
    {
        if(taskLogic)
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

    std::optional<MessageProcessor::Validity> MessageProcessor::processAnswer(
        const std::string &from, const std::string &answer)
    {
        if(taskLogic)
        {
            const auto ans = strutil::toCoreString(answer);
            auto res = answerTask(ans);
            if(res.valid)
            {
                sendValid(from, res.validityDescription, ans,
                    taskLogic->getStats());
            }
            else
            {
                sendInvalid(from, res.validityDescription, ans);
            }
            if(res.answer)
            {
                if(!res.valid)
                {
                    sendAnswer(*res.answer);
                }
                sendTask(*taskLogic);
            }
            return res.valid?VALID:INVALID;
        }
        send("no game is played now");
        sendNormHelp();
        return std::nullopt;
    }

    void MessageProcessor::processHintCmd(const std::string&,
        const StringList&)
    {
        if(taskLogic)
        {
            sendHint(*taskLogic);
        }
        else
        {
            send("no game is played now");
            sendNormHelp();
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
        {
            ss<<task<<std::endl;
        }
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
        std::stringstream ss;
        ss<<"TASK:"<<std::endl;
        ss<<strutil::fromCoreString(render.text());
        send(ss.str());
    }

    void MessageProcessor::sendHint(core::TaskLogic &logic)
    {
        StringRender render;
        logic.hint(render);
        std::stringstream ss;
        if(!render.text().empty())
        {
            ss<<"HINT: "<<strutil::fromCoreString(render.text());
        }
        else
        {
            ss<<"no more hints";
        }
        send(ss.str());
    }

    void MessageProcessor::sendInvalid(const std::string&,
        const std::optional<core::String> &descr, const core::String&)
    {
        std::wstringstream stream;
        stream<<"WRONG";
        if(descr)
        {
            stream<<": "<<*descr;
        }
        send(strutil::fromCoreString(std::move(stream).str()));
    }

    void MessageProcessor::sendValid(const std::string&,
        const std::optional<core::String>&,
        const core::String &answer, const core::TaskLogic::StatsCol &stats)
    {
        std::wstringstream stream;
        if(stats.empty())
        {
            throw std::runtime_error("stats list is empty on valid result");
        }
        const auto elapsed = stats.back().time;
        stream<<L"RIGHT: "<<answer
            <<" ("<<std::setprecision(2)<<std::fixed
            <<fractionalSeconds(elapsed)<<"s)";
        send(strutil::fromCoreString(stream.str()));
    }

    void MessageProcessor::sendAnswer(const core::String &str)
    {
        send(strutil::fromCoreString(L"ANSWER: " + str));
    }

    void MessageProcessor::sendStats(const core::TaskLogic::StatsCol &stats)
    {
        if(stats.empty())
        {
            return;
        }
        std::wstringstream stream;
        std::chrono::microseconds averageTime{};
        std::size_t answeredCount = 0;
        std::size_t wrongCount = 0;
        for(const auto &s : stats)
        {
            averageTime += s.time;
            if(s.answered)
            {
                answeredCount += 1;
            }
            wrongCount += s.tries;
        }
        averageTime /= stats.size();
        stream<<"stats:"<<std::endl;
        stream<<"played: "<<stats.size()<<" tasks"<<std::endl
            <<"answered: "<<answeredCount<<" tasks"<<std::endl
            <<"wrong: "<<wrongCount<<" answers"<<std::endl
            <<"average time: "<<std::setprecision(2)<<std::fixed
                <<fractionalSeconds(averageTime)<<"s";
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

    MessageProcessor::AnswerResult MessageProcessor::answerTask(
        const core::String &answer)
    {
        if(taskLogic)
        {
            auto res = taskLogic->validate(answer);
            return {
                res.validity.valid,
                std::move(res.validity.description),
                std::move(res.answer)
            };
        }
        return {false, {}, core::String()};
    }

    void MessageProcessor::quit()
    {
        taskLogic.reset();
    }

    core::String MessageProcessor::skip()
    {
        if(taskLogic)
        {
            return taskLogic->skip();
        }
        return {};
    }
}
