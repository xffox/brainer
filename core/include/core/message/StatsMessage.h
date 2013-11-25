#ifndef CORE_MESSAGE_STATSMESSAGE_H
#define CORE_MESSAGE_STATSMESSAGE_H

#include "engine/IMessage.h"
#include "core/MessageHandler.h"
#include "core/TaskId.h"
#include "core/Stats.h"

namespace core
{
    namespace message
    {
        class StatsMessage: public engine::IMessage<MessageHandler>
        {
        public:
            StatsMessage(TaskId taskId, const Stats &stats)
                :taskId(taskId), stats(stats)
            {}

            virtual std::auto_ptr<engine::IMessage<MessageHandler> > clone() const
            {
                return std::auto_ptr<engine::IMessage<MessageHandler> >(
                    new StatsMessage(*this));
            }

            virtual void handle(MessageHandler &handler) const
            {
                handler.handle(*this);
            }

            TaskId getTaskId() const
            {
                return taskId;
            }

            const Stats &getStats() const
            {
                return stats;
            }
            
        private:
            TaskId taskId;
            Stats stats;
        };
    }
}

#endif
