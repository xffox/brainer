#ifndef TASK_TASKPROVIDER_H
#define TASK_TASKPROVIDER_H

#include "core/ITaskProvider.h"

namespace task
{
    class TaskProvider: public core::ITaskProvider
    {
    public:
        TaskProvider();

        virtual StringSet getTasks() const;
        // throw exception
        virtual std::auto_ptr<core::ITaskGenerator> create(const std::string &name);

    private:
        StringSet tasks;
    };
}

#endif
