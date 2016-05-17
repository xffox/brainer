#ifndef FACT_LOGICTYPE_H
#define FACT_LOGICTYPE_H

#include <string>
#include <vector>

namespace fact
{
    class LogicType
    {
    public:
        using ParameterCol = std::vector<LogicType>;

        std::string name;
        ParameterCol parameters;
    };

    inline bool operator==(const LogicType &left, const LogicType &right)
    {
        return left.name == right.name &&
            left.parameters == right.parameters;
    }

    inline bool operator!=(const LogicType &left, const LogicType &right)
    {
        return !(left == right);
    }
}

#endif
