#ifndef FACT_LOGICFUNC_H
#define FACT_LOGICFUNC_H

#include <string>

#include "fact/LogicType.h"

namespace fact
{
    struct LogicFunc
    {
        using ArgumentCol = std::vector<LogicType>;

        LogicFunc(const std::string &name, const LogicType &result,
            const ArgumentCol &args)
            :name(name), result(result), args(args){}

        std::string name;
        LogicType result;
        ArgumentCol args;
    };
}

#endif
