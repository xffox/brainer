#include "task/MultiplicationTask.h"

#include <cstdlib>
#include <limits>
#include <sstream>
#include <stdexcept>

#include "core/IRender.h"
#include "base/strutil.h"
#include "util.h"

namespace task
{
    bool MultiplicationTask::validate(const core::String &result) const
    {
        int value = 0;
        std::string regStr;
        return util::toRegularStr(regStr, result) &&
            base::strutil::fromStr(value, regStr) &&
            value == product;
    }

    core::String MultiplicationTask::answer() const
    {
        return util::toWString(base::strutil::toStr(product));
    }

    void MultiplicationTask::describe(core::IRender &render) const
    {
        std::wstringstream stream;
        stream<<a<<'x'<<b;
        render.showTask(stream.str());
    }
}
