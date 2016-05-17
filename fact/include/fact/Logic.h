#ifndef FACT_LOGIC_H
#define FACT_LOGIC_H

#include <vector>

#include "fact/LogicFunc.h"

namespace fact
{
    class Logic
    {
    public:
        using FuncCol = std::vector<LogicFunc>;

    public:
        Logic(const FuncCol &functions)
            :functions(functions)
        {}

        LogicFunc generate() const;

    private:
        FuncCol functions;
    };
}

#endif
