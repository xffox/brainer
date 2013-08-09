#ifndef CORE_ITASKLOGICWATCHER_H
#define CORE_ITASKLOGICWATCHER_H

namespace core
{
    class ITask;

    class ITaskLogicWatcher
    {
    public:
        virtual ~ITaskLogicWatcher(){}
        
        virtual void onGenerated(ITask &task, ITask *prevTask) = 0;
        virtual void onResult(ITask &task, bool valid, long long elapsed) = 0;
    };
}

#endif
