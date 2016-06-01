#include "task/FactDescriptionTask.h"

#include <algorithm>
#include <cwctype>

#include "core/IRender.h"

#include "util.h"

namespace task
{
    FactDescriptionTask::FactDescriptionTask(const core::String &value,
        const core::String &description)
        :value(value), description(description)
    {
    }

    bool FactDescriptionTask::validate(const core::String &result) const
    {
        return util::tolower(result) == util::tolower(value);
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
