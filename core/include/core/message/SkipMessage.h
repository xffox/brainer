#ifndef CORE_MESSAGE_SKIPMESSAGE_H
#define CORE_MESSAGE_SKIPMESSAGE_H

#include "engine/IMessage.h"
#include "core/MessageHandler.h"
#include "core/TaskId.h"

namespace core
{
    namespace message
    {
        class SkipMessage: public engine::IMessage<MessageHandler>
        {
        public:
            SkipMessage(TaskId taskId)
                :taskId(taskId)
            {}

            virtual std::auto_ptr<engine::IMessage<MessageHandler> > clone() const
            {
                return std::auto_ptr<engine::IMessage<MessageHandler> >(
                    new SkipMessage(*this));
            }

            virtual void handle(MessageHandler &handler) const
            {
                handler.handle(*this);
            }

            TaskId getTaskId() const
            {
                return taskId;
            }

        private:
            TaskId taskId;
        };
    }
}

#endif
