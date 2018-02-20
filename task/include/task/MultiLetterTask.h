#ifndef TASK_MULTILETTERTASK_H
#define TASK_MULTILETTERTASK_H

#include "core/ITask.h"

namespace task
{
    class MultiLetterTask: public core::ITask
    {
    public:
        MultiLetterTask(core::String &&key, core::String &&value);

    public:
        virtual bool validate(const core::String &result) const override;
        virtual core::String answer() const override;
        virtual void describe(core::IRender &render) const override;
        virtual void hint(core::IRender &render, std::size_t level) const;

    private:
        core::String key;
        core::String value;
    };
}

#endif
