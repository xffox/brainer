#ifndef BOT_MESSAGEPROCESSOR_H
#define BOT_MESSAGEPROCESSOR_H

#include <memory>
#include <string>

#include "core/String.h"

#include "Command.h"
#include "StringList.h"

namespace core
{
    class TaskLogic;
}

namespace task
{
    class TaskProvider;
}

namespace bot
{
    class MessageProcessor
    {
    public:
        class Sender
        {
        public:
            virtual ~Sender(){}
            virtual void send(const std::string &msg) = 0;
        };

    public:
        MessageProcessor(Sender &sender, task::TaskProvider &taskProvider);
        virtual ~MessageProcessor();

        virtual void receive(const std::string &from, const std::string &msg);

    protected:
        virtual void processCommand(const std::string &from, const Command &command);
        virtual void processMessage(const std::string &from, const std::string &message);
        virtual void processListCmd(const std::string &from, const StringList &args);
        virtual void processHelpCmd(const std::string &from, const StringList &args);
        virtual void processPlayCmd(const std::string &from, const StringList &args);
        virtual void processQuitCmd(const std::string &from, const StringList &args);
        virtual void processSkipCmd(const std::string &from, const StringList &args);
        virtual void processAnswer(const std::string &from, const std::string &answer);

        void send(const std::string &msg);
        void sendTaskList();
        void sendNormHelp();
        void sendPlayHelp();

        void sendTask(core::TaskLogic &logic);
        void sendInvalid(const std::string &from, const core::String &descr,
            const core::String &str,
            core::TaskLogic &logic);
        void sendValid(const std::string &from, const core::String &descr,
            const core::String &str,
            core::TaskLogic &logic);
        void sendAnswer(const core::String &str);
        void sendStats(core::TaskLogic &logic);
        core::String description(core::TaskLogic &logic);

        core::TaskLogic *getTaskLogic() const;
        bool play(const std::string &name);
        bool answerTask(const core::String &answer);
        void quit();
        core::String skip();

    protected:
        Sender &sender;

    private:
        task::TaskProvider &taskProvider;
        std::unique_ptr<core::TaskLogic> taskLogic;
    };
}

#endif
