#ifndef CORE_MESSAGE_RESULTMESSAGE_H
#define CORE_MESSAGE_RESULTMESSAGE_H

#include <string>

#include "engine/IMessage.h"
#include "core/MessageHandler.h"
#include "core/TaskId.h"

namespace core
{
    namespace message
    {
        class ResultMessage: public engine::IMessage<MessageHandler>
        {
        public:
            ResultMessage(TaskId taskId, const std::string &value, bool valid)
                :taskId(taskId), value(value), valid(valid)
            {}

            virtual std::auto_ptr<engine::IMessage<MessageHandler> > clone() const
            {
                return std::auto_ptr<engine::IMessage<MessageHandler> >(
                    new ResultMessage(*this));
            }

            virtual void handle(MessageHandler &handler) const
            {
                handler.handle(*this);
            }

            TaskId getTaskID() const
            {
                return taskId;
            }

            const std::string &getValue() const
            {
                return value;
            }

            bool isValid() const
            {
                return valid;
            }

        private:
            TaskId taskId;
            std::string value;
            bool valid;
        };
    }
}

#endif
