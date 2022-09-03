#include "task/ArithmeticTask.h"

#include <cstdlib>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <cassert>

#include "core/IRender.h"
#include "base/strutil.h"
#include "util.h"

namespace task
{
    namespace
    {
        char operationString(ArithmeticTask::Operation op)
        {
            switch(op)
            {
            case ArithmeticTask::Operation::PLUS:
                return '+';
            case ArithmeticTask::Operation::MULT:
                return '*';
            }
            assert(false);
            return '?';
        }
    }

    ArithmeticTask::ArithmeticTask(Value a, Value b, Operation operation)
            :a(a), b(b), result(run(a, b, operation)), operation(operation)
        {}

    bool ArithmeticTask::validateBase(const core::String &result)
    {
        Value value = 0;
        std::string regStr;
        return util::toRegularStr(regStr, result) &&
            base::strutil::fromStr(value, regStr) &&
            value == this->result;
    }

    core::String ArithmeticTask::answer() const
    {
        return util::toWString(base::strutil::toStr(result));
    }

    void ArithmeticTask::describe(core::IRender &render) const
    {
        std::wstringstream stream;
        stream<<a<<operationString(operation)<<b;
        render.addText(stream.str());
    }

    void ArithmeticTask::hint(core::IRender&, std::size_t)
    {}

    ArithmeticTask::Value ArithmeticTask::run(Value a, Value b,
        ArithmeticTask::Operation operation)
    {
        switch(operation)
        {
        case ArithmeticTask::Operation::PLUS:
            return a+b;
        case ArithmeticTask::Operation::MULT:
            return a*b;
        }
        assert(false);
        return 0;
    }
}
