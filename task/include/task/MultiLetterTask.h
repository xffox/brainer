#ifndef TASK_MULTILETTERTASK_H
#define TASK_MULTILETTERTASK_H

#include "task/BaseTask.h"

namespace task
{
    class MultiLetterTask: public BaseTask
    {
    public:
        MultiLetterTask(core::String &&key, core::String &&value);

    public:
        bool validateBase(const core::String &result) override;
        core::String answer() const override;
        void describe(core::IRender &render) const override;
        void hint(core::IRender &render, std::size_t level) const override;

    private:
        core::String key;
        core::String value;
    };
}

#endif
