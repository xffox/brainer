#ifndef CORE_MESSAGE_ELAPSEDMESSAGE_H
#define CORE_MESSAGE_ELAPSEDMESSAGE_H

#include "engine/IMessage.h"
#include "core/MessageHandler.h"
#include "core/TaskId.h"

namespace core
{
    namespace message
    {
        class ElapsedMessage: public engine::IMessage<MessageHandler>
        {
        public:
            ElapsedMessage(TaskId taskId)
                :taskId(taskId), elapsed(0)
            {}

            virtual std::auto_ptr<engine::IMessage<MessageHandler> > clone() const
            {
                return std::auto_ptr<engine::IMessage<MessageHandler> >(
                    new ElapsedMessage(*this));
            }

            virtual void handle(MessageHandler &handler) const
            {
                handler.handle(*this);
            }

            TaskId getTaskId() const
            {
                return taskId;
            }

            long long getElapsed() const
            {
                return elapsed;
            }

            ElapsedMessage &setElapsed(long long elapsed)
            {
                this->elapsed = elapsed;
                return *this;
            }

        private:
            TaskId taskId;
            long long elapsed;
        };
    }
}

#endif
