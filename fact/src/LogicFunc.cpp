#include "fact/LogicFunc.h"

#include <stack>
#include <cassert>
#include <stdexcept>

namespace fact
{
    LogicFunc::LogicFunc(const std::string &name, const LogicType &res,
        const ArgumentCol &args)
        :name(name), root{res, NodeChildren()}, args(args)
    {
        root.arguments.reserve(args.size());
        for(const auto &t : args)
        {
            root.arguments.push_back(std::make_pair(t,
                    std::shared_ptr<Node>()));
        }
    }

    LogicFunc::LogicFunc(const LogicFunc &that)
        :name(that.name), root(*copy(that.root)), args(that.args)
    {}

    LogicFunc &LogicFunc::operator=(const LogicFunc &that)
    {
        name = that.name;
        root = *copy(that.root);
        args = that.args;
        return *this;
    }

    const LogicFunc::ArgumentCol &LogicFunc::arguments() const
    {
        return args;
    }

    const LogicType &LogicFunc::result() const
    {
        return root.result;
    }

    LogicFunc LogicFunc::dot(std::size_t argumentIdx,
        const LogicFunc &func) const
    {
        LogicFunc result(*this);
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

    LogicFunc::Child *LogicFunc::findArgument(std::size_t argumentIdx)
    {
        using NodeStack = std::stack<std::pair<Node*, std::size_t>>;
        NodeStack nodes;
        nodes.push(std::make_pair(&root, 0));
        std::size_t curIdx = 0;
        while(!nodes.empty())
        {
            auto &cur = nodes.top();
            assert(cur.first);
            for(auto &child : cur.first->arguments)
            {
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

    std::shared_ptr<LogicFunc::Node> LogicFunc::copy(const Node &node)
    {
        std::shared_ptr<Node> cur = std::shared_ptr<Node>(new Node());
        cur->result = node.result;
        cur->arguments.reserve(node.arguments.size());
        for(const auto &child : node.arguments)
        {
            cur->arguments.push_back(make_pair(child.first,
                    child.second.get()
                    ?copy(*child.second):std::shared_ptr<Node>()));
        }
        return cur;
    }
}
