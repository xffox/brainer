#ifndef TASK_FACTDESCRIPTIONTASK_H
#define TASK_FACTDESCRIPTIONTASK_H

#include "core/ITask.h"
#include "core/String.h"

namespace task
{
    class FactDescriptionTask: public core::ITask
    {
    public:
        FactDescriptionTask(const core::String &value,
            const core::String &description);

        virtual bool validate(const core::String &result) const override;
        virtual core::String answer() const override;
        virtual void describe(core::IRender &render) const override;

    private:
        core::String value;
        core::String description;
    };
}

#endif
