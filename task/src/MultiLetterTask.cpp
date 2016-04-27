#include "task/MultiLetterTask.h"

#include <cstdlib>

#include "core/IRender.h"

namespace task
{
    MultiLetterTask::MultiLetterTask(core::String &&key, core::String &&value)
        :key(std::move(key)), value(std::move(value))
    {}

    bool MultiLetterTask::validate(const core::String &result) const
    {
        return result == key;
    }

    core::String MultiLetterTask::answer() const
    {
        return key;
    }

    void MultiLetterTask::describe(core::IRender &render) const
    {
        render.addText(value);
    }
}
