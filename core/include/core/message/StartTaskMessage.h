#ifndef CORE_MESSAGE_STARTTASKMESSAGE_H
#define CORE_MESSAGE_STARTTASKMESSAGE_H

#include <string>

#include "engine/IMessage.h"
#include "core/MessageHandler.h"

namespace core
{
    namespace message
    {
        class StartTaskMessage: public engine::IMessage<MessageHandler>
        {
        public:
            StartTaskMessage(const std::string &task)
                :task(task)
            {}

            virtual std::auto_ptr<engine::IMessage<MessageHandler> > clone() const
            {
                return std::auto_ptr<engine::IMessage<MessageHandler> >(
                    new StartTaskMessage(*this));
            }

            virtual void handle(MessageHandler &handler) const
            {
                handler.handle(*this);
            }

            const std::string &getTask() const
            {
                return task;
            }

        private:
            std::string task;
        };
    }
}

#endif
