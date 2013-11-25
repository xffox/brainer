#ifndef CORE_MESSAGE_TASKSMESSAGE_H
#define CORE_MESSAGE_TASKSMESSAGE_H

#include <set>
#include <string>

#include "core/MessageHandler.h"
#include "engine/IMessage.h"

namespace core
{
    namespace message
    {
        class TasksMessage: public engine::IMessage<MessageHandler>
        {
        public:
            typedef std::set<std::string> StringSet;

        public:
            virtual std::auto_ptr<engine::IMessage<MessageHandler> > clone() const
            {
                return std::auto_ptr<engine::IMessage<MessageHandler> >(
                    new TasksMessage(*this));
            }

            virtual void handle(MessageHandler &handler) const
            {
                handler.handle(*this);
            }

            StringSet &getTasks()
            {
                return tasks;
            }

            const StringSet &getTasks() const
            {
                return const_cast<TasksMessage*>(this)->getTasks();
            }

        private:
            StringSet tasks;
        };
    }
}

#endif
