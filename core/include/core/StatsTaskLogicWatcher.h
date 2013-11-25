#ifndef CORE_STATSTASKLOGICWATCHER_H
#define CORE_STATSTASKLOGICWATCHER_H

#include <vector>

#include "core/ITaskLogicWatcher.h"

namespace core
{
    class StatsTaskLogicWatcher: public core::ITaskLogicWatcher
    {
    public:
        typedef std::vector<long long> TimeCollection;

    public:
        StatsTaskLogicWatcher();

        unsigned int getTasksCount() const
        {
            return tasksCount;
        }

        unsigned int getValidCount() const
        {
            return validTimes.size();
        }

        unsigned int getTriesCount() const
        {
            return triesCount;
        }

        const TimeCollection &getValidTimes() const
        {
            return validTimes;
        }

    protected:
        virtual void onGenerated(core::ITask &core, core::ITask *prevTask);
        virtual void onResult(core::ITask &core, bool validCount,
            long long elapsed);

    private:
        unsigned int tasksCount;
        unsigned int triesCount;
        TimeCollection validTimes;
    };
}

#endif
