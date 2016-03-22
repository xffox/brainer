#ifndef TASK_DICTTASKGENERATOR_H
#define TASK_DICTTASKGENERATOR_H

#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "core/ITaskGenerator.h"
#include "core/String.h"

namespace core
{
    class IDictProvider;
}

namespace task
{
    class DictTaskGenerator: public core::ITaskGenerator
    {
    public:
        typedef std::vector<std::pair<core::String, core::String>> TaskCollection;

    public:
        DictTaskGenerator(unsigned int seed, const TaskCollection &tasks);
        virtual ~DictTaskGenerator();

        virtual std::auto_ptr<core::ITask> generateTask();

    private:
        TaskCollection tasks;
    };
}

#endif
