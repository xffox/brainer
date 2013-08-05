#include "task/MultiplicationTask.h"

#include <cstdlib>
#include <limits>
#include <sstream>

#include "core/IRender.h"

namespace task
{
    bool MultiplicationTask::validate(const std::string &result)
    {
        const char *const str = result.c_str();
        char *endptr = 0;
        long res = std::strtol(str, &endptr, 10);
        if(endptr != 0 && *endptr == '\0')
        {
            if(res >= std::numeric_limits<int>::min() &&
                res <= std::numeric_limits<int>::max())
            {
                return res == product;
            }
        }
        return false;
    }

    void MultiplicationTask::describe(core::IRender &render)
    {
        std::stringstream stream;
        stream<<a<<'x'<<b;
        render.addText(stream.str());
    }
}
