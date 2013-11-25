#ifndef CORE_MESSSAGE_VALIDATEMESSAGE_H
#define CORE_MESSSAGE_VALIDATEMESSAGE_H

#include <string>

#include "engine/IMessage.h"
#include "core/MessageHandler.h"
#include "core/TaskId.h"

namespace core
{
    namespace message
    {
        class ValidateMessage: public engine::IMessage<MessageHandler>
        {
        public:
            ValidateMessage(TaskId taskId, const std::string &value)
                :taskId(taskId), value(value)
            {}

            virtual std::auto_ptr<engine::IMessage<MessageHandler> > clone() const
            {
                return std::auto_ptr<engine::IMessage<MessageHandler> >(
                    new ValidateMessage(*this));
            }

            virtual void handle(MessageHandler &handler) const
            {
                handler.handle(*this);
            }

            TaskId getTaskId() const
            {
                return taskId;
            }

            const std::string &getValue() const
            {
                return value;
            }
            
        private:
            TaskId taskId;
            std::string value;
        };
    }
}

#endif
