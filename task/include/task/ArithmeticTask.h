#ifndef TASK_MULTIPLICATIONTASK_H
#define TASK_MULTIPLICATIONTASK_H

#include "task/BaseTask.h"

namespace task
{
    class ArithmeticTask: public BaseTask
    {
        friend class ArithmeticTaskGenerator;
    public:
        enum class Operation
        {
            PLUS = 0,
            MULT
        };

    public:
        bool validateBase(const core::String &result) override;
        core::String answer() const override;
        void describe(core::IRender &render) const override;
        void hint(core::IRender &render, std::size_t level) const override;

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
