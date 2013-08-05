#ifndef TASK_HEXBYTETASKGENERATOR_H
#define TASK_HEXBYTETASKGENERATOR_H

#include "core/ITaskGenerator.h"

namespace task
{
    // TODO: outer random generator
    class HexByteTaskGenerator: public core::ITaskGenerator
    {
    public:
        HexByteTaskGenerator(unsigned int seed);

        virtual std::auto_ptr<core::ITask> generateTask();
    };
}

#endif
