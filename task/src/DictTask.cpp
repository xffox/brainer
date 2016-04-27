#include "task/DictTask.h"

#include <sstream>
#include <iterator>
#include <stdexcept>

#include "base/strutil.h"
#include "core/IRender.h"

#include "util.h"

namespace task
{
    DictTask::DictTask(const StringCollection &keys, const StringCollection &values)
        :keys(keys), values(values), answers()
    {
        if(values.empty() || keys.empty())
            throw std::runtime_error("task values or keys are empty");
        util::toLowerCollection(keys.begin(), keys.end(),
            std::inserter(answers, answers.begin()));
    }

    bool DictTask::validate(const core::String &result) const
    {
        return answers.find(util::tolower(result)) != answers.end();
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
