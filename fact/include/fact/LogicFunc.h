#ifndef FACT_LOGICFUNC_H
#define FACT_LOGICFUNC_H

#include <vector>
#include <memory>
#include <utility>
#include <string>

#include "fact/LogicType.h"

namespace fact
{
    class LogicFunc
    {
    public:
        using ArgumentCol = std::vector<LogicType>;

    public:
        LogicFunc(const std::string &name, const LogicType &res,
            const ArgumentCol &args);
        LogicFunc(const LogicFunc &that);
        LogicFunc &operator=(const LogicFunc &that);

        const ArgumentCol &arguments() const;
        const LogicType &result() const;

        LogicFunc dot(std::size_t argumentIdx, const LogicFunc &func) const;

    private:
        struct Node;
        using Child = std::pair<LogicType, std::shared_ptr<Node>>;
        using NodeChildren = std::vector<Child>;
        struct Node
        {
            LogicType result;
            NodeChildren arguments;
        };

    private:
        Child *findArgument(std::size_t argumentIdx);

        static std::shared_ptr<Node> copy(const Node &node);

    private:
        std::string name;
        Node root;
        ArgumentCol args;
    };
}

#endif
