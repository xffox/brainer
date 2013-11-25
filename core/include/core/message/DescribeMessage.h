#ifndef CORE_MESSAGE_DESCRIBEMESSAGE_H
#define CORE_MESSAGE_DESCRIBEMESSAGE_H

#include <string>

#include "engine/IMessage.h"
#include "core/MessageHandler.h"
#include "core/TaskId.h"

namespace core
{
    namespace message
    {
        class DescribeMessage: public engine::IMessage<MessageHandler>
        {
        public:
            DescribeMessage(TaskId taskId, const std::string &description)
                :taskId(taskId), description(description)
            {}

            virtual std::auto_ptr<engine::IMessage<MessageHandler> > clone() const
            {
                return std::auto_ptr<engine::IMessage<MessageHandler> >(
                    new DescribeMessage(*this));
            }

            virtual void handle(MessageHandler &handler) const
            {
                handler.handle(*this);
            }

            TaskId getTaskId() const
            {
                return taskId;
            }

            const std::string &getDescription() const
            {
                return description;
            }

        private:
            TaskId taskId;
            std::string description;
        };
    }
}

#endif
