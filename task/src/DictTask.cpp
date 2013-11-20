#include "task/DictTask.h"

#include "core/IRender.h"

namespace task
{
    DictTask::DictTask(const std::string &key, const std::string &value)
        :key(key), value(value)
    {
    }

    bool DictTask::validate(const ValueType &result)
    {
        return result.str() == value;
    }

    void DictTask::describe(core::IRender &render)
    {
        render.addText(key);
    }
}
