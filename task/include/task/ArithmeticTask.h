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
        [[nodiscard]]
        core::String answer() const override;
        void describe(core::IRender &render) const override;
        void hint(core::IRender &render, std::size_t level) override;

    private:
        using Value = int;

    private:
        ArithmeticTask(Value a, Value b, Operation op);

        static Value run(Value a, Value b, Operation operation);

    private:
        Value a;
        Value b;
        Value result;
        Operation operation;
    };
}

#endif
