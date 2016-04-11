#include "task/DictTask.h"

#include <sstream>
#include <stdexcept>

#include "base/strutil.h"
#include "core/IRender.h"

namespace task
{
    DictTask::DictTask(const StringCollection &keys, const StringCollection &values)
        :keys(keys), values(values), answers(keys.begin(), keys.end())
    {
        if(values.empty())
            throw std::runtime_error("task values are empty");
    }

    bool DictTask::validate(const core::String &result) const
    {
        return answers.find(result) != answers.end();
    }

    core::String DictTask::answer() const
    {
        return base::strutil::join(keys.begin(), keys.end(),
            core::String(L","));
    }

    void DictTask::describe(core::IRender &render) const
    {
        render.addText(base::strutil::join(values.begin(), values.end(),
                core::String(L",")));
    }
}
