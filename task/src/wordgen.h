#ifndef TASK_WORDGEN_H
#define TASK_WORDGEN_H

#include <memory>

#include "core/ITaskGenerator.h"
#include "task/TaskProvider.h"

namespace task
{
    namespace wordgen
    {
        std::unique_ptr<core::ITaskGenerator> createTaskGenerator(
            const TaskProvider::StringCol &params);
    }
}

#endif
