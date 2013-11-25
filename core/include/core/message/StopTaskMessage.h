#ifndef CORE_MESSAGE_STOPTASKMESSAGE_H
#define CORE_MESSAGE_STOPTASKMESSAGE_H

#include "engine/IMessage.h"
#include "core/MessageHandler.h"
#include "core/TaskId.h"

namespace core
{
    namespace message
    {
        class StopTaskMessage: public engine::IMessage<MessageHandler>
        {
        public:
            StopTaskMessage(TaskId taskId)
                :taskId(taskId)
            {}

            virtual std::auto_ptr<engine::IMessage<MessageHandler> > clone() const
            {
                return std::auto_ptr<engine::IMessage<MessageHandler> >(
                    new StopTaskMessage(*this));
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
