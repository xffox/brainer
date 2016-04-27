#include "task/FactTask.h"

#include <sstream>
#include <iterator>

#include "base/strutil.h"
#include "core/IRender.h"

#include "util.h"

namespace task
{
    FactTask::FactTask(const core::String &item, const core::String &property,
        const StringSet &values)
        :item(item), property(property), values(values), lowercaseValues()
    {
        util::toLowerCollection(values.begin(), values.end(),
            std::inserter(lowercaseValues, lowercaseValues.begin()));
    }

    bool FactTask::validate(const core::String &result) const
    {
        const auto words = base::strutil::split(result.begin(), result.end(),
            L',');
        StringSet answerValues;
        util::toLowerCollection(words.begin(), words.end(),
            std::inserter(answerValues, answerValues.begin()));
        return answerValues == lowercaseValues;
    }

    core::String FactTask::answer() const
    {
        return base::strutil::join(values.begin(), values.end(),
            core::String(L","));
    }

    void FactTask::describe(core::IRender &render) const
    {
        std::wstringstream ss;
        ss<<item<<" -> "<<property<<" -> "<<(values.size()>1?"{?}":"?");
        render.addText(ss.str());
    }
}
