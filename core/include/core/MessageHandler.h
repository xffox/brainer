#ifndef CORE_MESSAGEHANDLER_H
#define CORE_MESSAGEHANDLER_H

namespace core
{
    namespace message
    {
        class TasksMessage;
        class StartTaskMessage;
        class StopTaskMessage;
        class DescribeMessage;
        class ValidateMessage;
        class ResultMessage;
        class SkipMessage;
        class StatsMessage;
        class ElapsedMessage;
    }

    class MessageHandler
    {
    public:
        virtual ~MessageHandler(){}

        virtual void handle(const message::TasksMessage&)
        {}
        virtual void handle(const message::StartTaskMessage&)
        {}
        virtual void handle(const message::StopTaskMessage&)
        {}
        virtual void handle(const message::DescribeMessage&)
        {}
        virtual void handle(const message::ValidateMessage&)
        {}
        virtual void handle(const message::ResultMessage&)
        {}
        virtual void handle(const message::SkipMessage&)
        {}
        virtual void handle(const message::StatsMessage&)
        {}
        virtual void handle(const message::ElapsedMessage&)
        {}
    };
}

#endif
