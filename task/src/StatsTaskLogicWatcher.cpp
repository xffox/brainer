#include "task/StatsTaskLogicWatcher.h"

namespace task
{
    StatsTaskLogicWatcher::StatsTaskLogicWatcher()
        :tasksCount(0), triesCount(0), validTimes()
    {
    }

    void StatsTaskLogicWatcher::onGenerated(core::ITask&,
        core::ITask*)
    {
        ++tasksCount;
    }

    void StatsTaskLogicWatcher::onResult(core::ITask&, bool valid,
        long long elapsed)
    {
        if(valid)
            validTimes.push_back(elapsed);
        ++triesCount;
    }
}
