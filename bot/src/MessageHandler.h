#ifndef BOT_MESSAGEHANDLER_H
#define BOT_MESSAGEHANDLER_H

#include <memory>

#include <gloox/messagehandler.h>

#include "task/TaskProvider.h"
#include "core/TaskLogic.h"
#include "core/String.h"

namespace bot
{
    class MessageHandler: public gloox::MessageHandler
    {
    public:
        MessageHandler(gloox::MessageSession &session,
            task::TaskProvider &taskProvider);

    protected:
        virtual void handleMessage(const gloox::Message& msg,
            gloox::MessageSession *session);

    private:
        bool playTasks(const std::string &name);
        void quitTasks();

        void sendTask(core::TaskLogic &logic);
        void sendInvalid(core::TaskLogic &logic, const core::String &str);
        void sendValid(core::TaskLogic &logic, const core::String &str);
        void sendAnswer(const core::String &str);
        core::String description(core::TaskLogic &logic);

        void listTasks();
        void sendNormHelp();
        void sendPlayHelp();

    private:
        gloox::MessageSession &session;
        task::TaskProvider &taskProvider;

        std::unique_ptr<core::TaskLogic> taskLogic;
    };
}

#endif
