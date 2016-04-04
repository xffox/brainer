#ifndef CORE_TASKLOGIC_H
#define CORE_TASKLOGIC_H

#include <memory>
#include <vector>

#include "base/Nullable.h"
#include "core/String.h"
#include "core/Stats.h"

namespace core
{
    class IRender;
    class ITask;
    class ITaskGenerator;
    class ITaskLogicWatcher;
    class Stopwatch;

    class TaskLogic
    {
    public:
        using StatsCol = std::vector<Stats>;

    public:
        TaskLogic(std::unique_ptr<ITaskGenerator> taskGenerator,
            IRender &render);
        ~TaskLogic();

        bool validate(const String &result);
        void skip();

        long long elapsed() const;

        const StatsCol &getStats() const;

    private:
        void generate();
        
    private:
        IRender &render;

        std::unique_ptr<Stopwatch> stopwatch;

        std::unique_ptr<ITaskGenerator> taskGenerator;

        std::unique_ptr<ITask> currentTask;

        // TODO: put to stats, use last as current
        base::Nullable<Stats> current;
        StatsCol stats;
    };
}

#endif
