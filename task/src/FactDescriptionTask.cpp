#include "task/FactDescriptionTask.h"

#include "core/IRender.h"

namespace task
{
    bool FactDescriptionTask::validate(const core::String &result) const
    {
        return result == value;
    }

    core::String FactDescriptionTask::answer() const
    {
        return value;
    }

    void FactDescriptionTask::describe(core::IRender &render) const
    {
        render.addText(description);
    }
}
