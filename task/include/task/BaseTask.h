#ifndef TASK_BASETASK_H
#define TASK_BASETASK_H

#include "core/ITask.h"
#include "core/String.h"

namespace task
{
    class BaseTask: public core::ITask
    {
    public:
        ValidationResult validate(const core::String &result) override
        {
            if(!wasValid)
            {
                wasValid = validateBase(result);
                return {wasValid, {}};
            }
            return {false, {}};
        }

        bool done() const override
        {
            return wasValid;
        }

    protected:
        virtual bool validateBase(const core::String &result) = 0;

    private:
        bool wasValid = false;
    };
}

#endif
