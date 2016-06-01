#ifndef TASK_DICTTASKGENERATOR_H
#define TASK_DICTTASKGENERATOR_H

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <cstddef>

#include "core/ITaskGenerator.h"
#include "core/String.h"
#include "task/StringCollection.h"
#include "IndexGenerator.h"

namespace core
{
    class IDictProvider;
}

namespace task
{
    class DictTaskGenerator: public core::ITaskGenerator
    {
    public:
        typedef std::vector<std::pair<StringCollection, StringCollection>> TaskCollection;

    public:
        DictTaskGenerator(unsigned int seed, const TaskCollection &tasks,
            bool reversed = false);
        virtual ~DictTaskGenerator();

        virtual std::unique_ptr<core::ITask> generateTask();

    private:
        TaskCollection tasks;
        bool reversed;
        IndexGenerator indexGenerator;
    };
}

#endif
