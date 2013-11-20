#ifndef TASK_HEXBYTETASK_H
#define TASK_HEXBYTETASK_H

#include "core/ITask.h"

namespace task
{
    class HexByteTask: public core::ITask
    {
        friend class HexByteTaskGenerator;
    public:
        virtual bool validate(const ValueType &result);
        virtual void describe(core::IRender &render);

    private:
        HexByteTask(unsigned char value)
            :value(value)
        {}

    private:
        unsigned char value;
    };
}

#endif
