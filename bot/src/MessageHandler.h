#ifndef BOT_MESSAGEHANDLER_H
#define BOT_MESSAGEHANDLER_H

#include <memory>

#include <gloox/messagehandler.h>

#include "task/TaskProvider.h"
#include "core/TaskLogic.h"
#include "core/IRender.h"

namespace bot
{
    class MessageHandler: public gloox::MessageHandler, public core::IRender
    {
    public:
        MessageHandler(gloox::MessageSession *session);

    protected:
        virtual void handleMessage(const gloox::Message& msg,
            gloox::MessageSession *session);

        virtual void showTask(const core::String &str);
        virtual void showInvalid(const core::String &str);
        virtual void showAnswer(const core::String &str);

    private:
        bool playTasks(const std::string &name);
        void quitTasks();
        void listTasks();

        void sendNormHelp();
        void sendPlayHelp();

    private:
        gloox::MessageSession *session;

        std::unique_ptr<task::TaskProvider> taskProvider;
        std::unique_ptr<core::TaskLogic> taskLogic;
    };
}

#endif
