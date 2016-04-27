#ifndef TASK_TASKPROVIDER_H
#define TASK_TASKPROVIDER_H

#include <unordered_map>
#include <functional>
#include <string>

#include "core/ITaskProvider.h"

namespace task
{
    class TaskProvider: public core::ITaskProvider
    {
    public:
        TaskProvider(const std::string &configFilename);

        virtual StringSet getTasks() const;
        // throw exception
        virtual std::unique_ptr<core::ITaskGenerator> create(
            const std::string &name);

    private:
        using TaskMap = std::unordered_map<std::string,
              std::function<std::unique_ptr<core::ITaskGenerator>()>>;

    private:
        TaskMap tasks;
    };
}

#endif
