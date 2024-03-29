#include "task/MultiLetterTask.h"

#include <cstdlib>

#include "core/IRender.h"

namespace task
{
    MultiLetterTask::MultiLetterTask(core::String &&key, core::String &&value)
        :key(std::move(key)), value(std::move(value))
    {}

    bool MultiLetterTask::validateBase(const core::String &result)
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

    void MultiLetterTask::hint(core::IRender&, std::size_t)
    {}
}
