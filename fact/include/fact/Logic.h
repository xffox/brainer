#ifndef FACT_LOGIC_H
#define FACT_LOGIC_H

#include <vector>

#include "fact/Expression.h"

namespace fact
{
    class Logic
    {
    public:
        using ExpressionCol = std::vector<Expression>;

    public:
        Logic(const ExpressionCol &functions);

        Expression generate() const;

    private:
        ExpressionCol functions;
    };
}

#endif
