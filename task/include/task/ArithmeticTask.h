#ifndef TASK_MULTIPLICATIONTASK_H
#define TASK_MULTIPLICATIONTASK_H

#include "core/ITask.h"

namespace task
{
    class ArithmeticTask: public core::ITask
    {
        friend class ArithmeticTaskGenerator;
    public:
        enum class Operation
        {
            PLUS = 0,
            MULT
        };

    public:
        virtual bool validate(const core::String &result) const;
        virtual core::String answer() const;
        virtual void describe(core::IRender &render) const;

    private:
        ArithmeticTask(int a, int b, Operation op);
        
    private:
        int a;
        int b;
        int result;
        Operation operation;
    };
}

#endif
