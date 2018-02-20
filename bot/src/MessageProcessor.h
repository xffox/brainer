#ifndef BOT_MESSAGEPROCESSOR_H
#define BOT_MESSAGEPROCESSOR_H

#include <memory>
#include <string>

#include "base/Nullable.h"
#include "core/String.h"
#include "core/TaskLogic.h"

#include "Command.h"
#include "StringList.h"

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
        enum Validity
        {
            INVALID,
            VALID
        };

    protected:
        virtual void processCommand(const std::string &from, const Command &command);
        virtual void processMessage(const std::string &from, const std::string &message);

        virtual void processListCmd(const std::string &from, const StringList &args);
        virtual void processHelpCmd(const std::string &from, const StringList &args);
        virtual void processPlayCmd(const std::string &from, const StringList &args);
        virtual void processQuitCmd(const std::string &from, const StringList &args);
        virtual void processSkipCmd(const std::string &from, const StringList &args);
        virtual void processHintCmd(const std::string &from, const StringList &args);
        virtual base::Nullable<Validity> processAnswer(const std::string &from, const std::string &answer);

        virtual void sendTaskList();
        virtual void sendNormHelp();
        virtual void sendPlayHelp();
        virtual void sendTask(core::TaskLogic &logic);
        virtual void sendHint(core::TaskLogic &logic);
        virtual void sendInvalid(const std::string &from, const core::String &descr,
            const core::String &str);
        virtual void sendValid(const std::string &from, const core::String &descr,
            const core::String &str, const core::TaskLogic::StatsCol &stats);
        virtual void sendAnswer(const core::String &str);
        virtual void sendStats(const core::TaskLogic::StatsCol &stats);

        core::TaskLogic *getTaskLogic() const;
        bool play(const std::string &name);
        bool answerTask(const core::String &answer);
        void quit();
        core::String skip();

        void send(const std::string &msg);

        core::String description(core::TaskLogic &logic);

    protected:
        Sender &sender;

    private:
        task::TaskProvider &taskProvider;
        std::unique_ptr<core::TaskLogic> taskLogic;
    };
}

#endif
