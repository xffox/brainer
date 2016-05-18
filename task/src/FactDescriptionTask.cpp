#include "task/FactDescriptionTask.h"

#include <algorithm>
#include <cwctype>

#include "core/IRender.h"

#include "util.h"

namespace task
{
    namespace
    {
        core::String replace(const core::String &str,
            const core::String &pattern, const core::String &replacement)
        {
            core::String result;
            auto cur = str.begin();
            while(cur != str.end())
            {
                auto patternIter = std::search(cur, str.end(),
                    pattern.begin(), pattern.end(),
                    [](core::String::value_type left, core::String::value_type right) {
                        return std::tolower(left) == std::tolower(right);
                    });
                result.insert(result.end(), cur, patternIter);
                if(patternIter != str.end())
                {
                    result.insert(result.end(), replacement.begin(), replacement.end());
                    cur = patternIter + pattern.size();
                }
                else
                {
                    cur = patternIter;
                }
            }
            return result;
        }
    }

    FactDescriptionTask::FactDescriptionTask(const core::String &value,
        const core::String &description)
        :value(util::tolower(value)), description(replace(description, value, L"?"))
    {
    }

    bool FactDescriptionTask::validate(const core::String &result) const
    {
        return util::tolower(result) == value;
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
