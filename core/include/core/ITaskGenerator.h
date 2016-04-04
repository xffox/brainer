#ifndef CORE_ITASKGENERATOR_H
#define CORE_ITASKGENERATOR_H

#include <memory>

namespace core
{
    class ITask;

    class ITaskGenerator
    {
    public:
        virtual ~ITaskGenerator(){}

        virtual std::unique_ptr<ITask> generateTask() = 0;
    };
}

#endif
