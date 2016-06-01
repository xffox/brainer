#ifndef FACT_EXPRESSION_H
#define FACT_EXPRESSION_H

#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <cstddef>
#include <unordered_map>

#include "fact/LogicType.h"
#include "fact/LogicFunc.h"

namespace fact
{
    class Expression
    {
    public:
        class IVisitor
        {
        public:
            using Idx = std::size_t;
            using ArgIdxMap = std::unordered_map<std::size_t, std::size_t>;

        public:
            virtual ~IVisitor(){}

            virtual void visit(const LogicFunc &func, Idx funcIdx,
                const ArgIdxMap &argumentIndices) = 0;
        };

    public:
        Expression(const LogicFunc &func);
        Expression(const Expression &that);
        Expression &operator=(const Expression &that);

        const LogicFunc::ArgumentCol &arguments() const;
        const LogicType &result() const;

        Expression dot(std::size_t argumentIdx, const Expression &func) const;

        void visit(IVisitor &visitor) const;

    private:
        struct Node;
        using Child = std::pair<LogicType, std::shared_ptr<Node>>;
        using NodeChildren = std::vector<Child>;
        struct Node
        {
            LogicFunc func;
            NodeChildren arguments;
        };

    private:
        Child *findArgument(std::size_t argumentIdx);

        static std::shared_ptr<Node> copy(const Node &node);

    private:
        Node root;
        LogicFunc::ArgumentCol args;
    };
}

#endif
