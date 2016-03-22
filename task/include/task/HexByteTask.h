#ifndef TASK_HEXBYTETASK_H
#define TASK_HEXBYTETASK_H

#include "core/ITask.h"

namespace task
{
    class HexByteTask: public core::ITask
    {
        friend class HexByteTaskGenerator;
    public:
        virtual bool validate(const core::String &result) const;
        virtual core::String answer() const;
        virtual void describe(core::IRender &render) const;

    private:
        HexByteTask(unsigned char value)
            :value(value)
        {}

    private:
        unsigned char value;
    };
}

#endif
