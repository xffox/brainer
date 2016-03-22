#include "task/DictTask.h"

#include "core/IRender.h"

namespace task
{
    DictTask::DictTask(const core::String &key, const core::String &value)
        :key(key), value(value)
    {
    }

    bool DictTask::validate(const core::String &result) const
    {
        return result == value;
    }

    core::String DictTask::answer() const
    {
        return value;
    }

    void DictTask::describe(core::IRender &render) const
    {
        render.showTask(key);
    }
}
