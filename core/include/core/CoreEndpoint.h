#ifndef CORE_COREENDPOINT_H
#define CORE_COREENDPOINT_H

#include <memory>
#include <string>

#include "engine/Endpoint.h"
#include "core/MessageHandler.h"

namespace core
{
    class TaskLogic;
    class ITaskGenerator;
    class ITaskProvider;
    class StatsTaskLogicWatcher;

    class CoreEndpoint: public engine::Endpoint<MessageHandler>,
        public MessageHandler
    {
    public:
        CoreEndpoint(std::auto_ptr<ITaskProvider> taskProvider);
        virtual ~CoreEndpoint();

        virtual MessageHandler &handler()
        {
            return *this;
        }

        virtual void handle(const message::StartTaskMessage &message);
        virtual void handle(const message::StopTaskMessage &message);
        virtual void handle(const message::ValidateMessage &message);
        virtual void handle(const message::SkipMessage &message);
        virtual void handle(const message::ElapsedMessage &message);

    protected:
        virtual void started();
        virtual void stopped();

    private:
        std::auto_ptr<ITaskGenerator> createGenerator(
            const std::string &task) const;

        void generate();
        void stats();

    private:
        std::auto_ptr<ITaskProvider> taskProvider;
        std::auto_ptr<StatsTaskLogicWatcher> statsWatcher;
        std::auto_ptr<TaskLogic> taskLogic;
    };
}

#endif
