#ifndef TASK_TASKPROVIDER_H
#define TASK_TASKPROVIDER_H

#include <unordered_map>
#include <functional>
#include <string>
#include <vector>

#include <base/randomizer.hpp>
#include "core/ITaskProvider.h"

namespace task
{
    class TaskProvider: public core::ITaskProvider
    {
    public:
        using StringCol = std::vector<std::string>;

    public:
        TaskProvider(base::Randomizer &&random,
            const std::string &configFilename);

        StringSet getTasks() const override;
        // throw exception
        std::unique_ptr<core::ITaskGenerator> create(
            const std::string &name) override;

    private:
        using TaskTypeCreatorMap = std::unordered_map<std::string,
              std::function<std::unique_ptr<core::ITaskGenerator>(const StringCol&)>>;

    private:
        std::string configFilename;
        base::Randomizer rnd;
        TaskTypeCreatorMap creators;
    };
}

#endif
