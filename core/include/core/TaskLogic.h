#ifndef CORE_TASKLOGIC_H
#define CORE_TASKLOGIC_H

#include <memory>
#include <vector>
#include <cstddef>
#include <optional>

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

        struct Validity
        {
            bool valid;
            std::optional<String> description;
        };

        struct ValidationResult
        {
            Validity validity;
            std::optional<String> answer;
        };

    public:
        explicit TaskLogic(std::unique_ptr<ITaskGenerator> taskGenerator);
        ~TaskLogic();

        ValidationResult validate(const String &result);
        void describe(IRender &render);
        String skip();
        void hint(IRender &render);

        long long elapsedUs() const;

        const StatsCol &getStats() const;

    private:
        void generate();

    private:
        std::unique_ptr<Stopwatch> stopwatch;

        std::unique_ptr<ITaskGenerator> taskGenerator;

        std::unique_ptr<ITask> currentTask;

        // TODO: put to stats, use last as current
        base::Nullable<Stats> current;
        StatsCol stats;
        std::size_t hintLevel;
    };
}

#endif
