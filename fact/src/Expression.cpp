#include "fact/Expression.h"

#include <stack>
#include <cassert>
#include <stdexcept>
#include <queue>
#include <stack>
#include <utility>

namespace fact
{
    Expression::Expression(const LogicFunc &func)
        :root{func, NodeChildren()}, args(func.args)
    {
        root.arguments.reserve(args.size());
        for(const auto &t : args)
        {
            root.arguments.push_back(std::make_pair(t,
                    std::shared_ptr<Node>()));
        }
    }

    Expression::Expression(const Expression &that)
        :root(*copy(that.root)), args(that.args)
    {}

    Expression &Expression::operator=(const Expression &that)
    {
        root = *copy(that.root);
        args = that.args;
        return *this;
    }

    const LogicFunc::ArgumentCol &Expression::arguments() const
    {
        return args;
    }

    const LogicType &Expression::result() const
    {
        return root.func.result;
    }

    Expression Expression::dot(std::size_t argumentIdx,
        const Expression &func) const
    {
        Expression result(*this);
        auto *const child = result.findArgument(argumentIdx);
        if(!child)
            throw std::invalid_argument("invalid argument index");
        if(child->first != func.result())
            throw std::invalid_argument("invalid result type");
        child->second = copy(func.root);
        const auto &funcArguments = func.arguments();
        result.args.erase(result.args.begin()+argumentIdx);
        result.args.insert(result.args.begin()+argumentIdx,
            funcArguments.begin(), funcArguments.end());
        return result;
    }

    Expression::Child *Expression::findArgument(std::size_t argumentIdx)
    {
        using NodeStack = std::stack<std::pair<Node*, std::size_t>>;
        NodeStack nodes;
        nodes.push(std::make_pair(&root, 0));
        std::size_t curIdx = 0;
        while(!nodes.empty())
        {
            auto &cur = nodes.top();
            assert(cur.first);
            if(cur.second == cur.first->arguments.size())
            {
                nodes.pop();
                continue;
            }
            while(cur.second < cur.first->arguments.size())
            {
                auto &child = cur.first->arguments[cur.second];
                ++cur.second;
                if(!child.second.get())
                {
                    if(curIdx == argumentIdx)
                        return &child;
                    ++curIdx;
                }
                else
                {
                    nodes.push(std::make_pair(child.second.get(), 0));
                    break;
                }
            }
        }
        return nullptr;
    }

    std::shared_ptr<Expression::Node> Expression::copy(const Node &node)
    {
        std::shared_ptr<Node> cur = std::shared_ptr<Node>(new Node{node.func, NodeChildren()});
        cur->arguments.reserve(node.arguments.size());
        for(const auto &child : node.arguments)
        {
            cur->arguments.push_back(make_pair(child.first,
                    child.second.get()
                    ?copy(*child.second):std::shared_ptr<Node>()));
        }
        return cur;
    }

    void Expression::visit(IVisitor &visitor) const
    {
        struct VisitNode
        {
            const Node *node;
            IVisitor::Idx idx;
            IVisitor::ArgIdxMap argumentIndices;
        };
        using Queue = std::queue<VisitNode>;
        using Stack = std::stack<VisitNode>;
        std::size_t curIdx = 0;
        Stack nodes;
        Queue unprocessedNodes;
        unprocessedNodes.push(VisitNode{&root, curIdx++,
            IVisitor::ArgIdxMap()});
        while(!unprocessedNodes.empty())
        {
            const auto curNode = unprocessedNodes.front();
            unprocessedNodes.pop();
            assert(curNode.node);
            IVisitor::ArgIdxMap argumentIndices;
            for(auto iter = curNode.node->arguments.rbegin();
                iter != curNode.node->arguments.rend(); ++iter)
            {
                if(!iter->second.get())
                    continue;
                const auto idx = curIdx++;
                argumentIndices.insert(std::make_pair(
                        curNode.node->arguments.size()-1 -
                        (iter - curNode.node->arguments.rbegin()),
                        idx));
                unprocessedNodes.push(VisitNode{iter->second.get(), idx,
                    IVisitor::ArgIdxMap()});
            }
            nodes.push(VisitNode{curNode.node, curNode.idx, argumentIndices});
        }
        while(!nodes.empty())
        {
            const auto cur = nodes.top();
            nodes.pop();
            assert(cur.node);
            visitor.visit(cur.node->func, cur.idx, cur.argumentIndices);
        }
    }
}
