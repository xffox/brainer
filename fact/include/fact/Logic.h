#ifndef FACT_LOGIC_H
#define FACT_LOGIC_H

#include <vector>

#include "fact/Expression.h"
#include "fact/LogicType.h"

namespace fact
{
    class Logic
    {
    public:
        using ExpressionCol = std::vector<Expression>;

    public:
        Expression generate(const ExpressionCol &functions) const;
        ExpressionCol generate(const ExpressionCol &functions,
            const LogicType::ParameterCol &availableParams) const;

    private:
        ExpressionCol functions;
    };
}

#endif
