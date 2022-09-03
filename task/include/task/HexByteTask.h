#ifndef TASK_HEXBYTETASK_H
#define TASK_HEXBYTETASK_H

#include "task/BaseTask.h"

namespace task
{
    class HexByteTask: public BaseTask
    {
        friend class HexByteTaskGenerator;
    public:
        bool validateBase(const core::String &result) override;
        [[nodiscard]]
        core::String answer() const override;
        void describe(core::IRender &render) const override;
        void hint(core::IRender &render, std::size_t level) override;

    private:
        using Value = unsigned char;

    private:
        explicit HexByteTask(Value value)
            :value(value)
        {}

    private:
        Value value;
    };
}

#endif
