#include "task/MultiplicationTask.h"

#include <cstdlib>
#include <limits>
#include <sstream>

#include "core/IRender.h"

namespace task
{
    bool MultiplicationTask::validate(const ValueType &result)
    {
        int value = 0;
        if(result.extract(value))
            return value == product;
        return false;
    }

    void MultiplicationTask::describe(core::IRender &render)
    {
        std::stringstream stream;
        stream<<a<<'x'<<b;
        render.addText(stream.str());
    }
}
