#ifndef TASK_FACTTASK_H
#define TASK_FACTTASK_H

#include "task/FactTask.h"
#include "core/ITask.h"
#include "task/StringSet.h"

namespace task
{
    class FactTask: public core::ITask
    {
    public:
        FactTask(const core::String &item, const core::String &property,
            const StringSet &values);

        bool validate(const core::String &result) const override;
        core::String answer() const override;
        void describe(core::IRender &render) const override;

    private:
        core::String item;
        core::String property;
        StringSet values;
        StringSet lowercaseValues;
    };
}

#endif
