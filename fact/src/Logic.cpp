#include "fact/Logic.h"

#include <algorithm>
#include <iterator>
#include <vector>
#include <cstddef>
#include <random>
#include <stdexcept>
#include <cassert>

#include "fact/LogicType.h"

namespace fact
{
    Logic::Logic(const ExpressionCol &functions)
        :functions(functions)
    {
        if(functions.empty())
            throw std::invalid_argument("empty functions list");
    }

    // TODO: optimal search in case of missing ordering of a type
    Expression Logic::generate() const
    {
        using LogicTypeCol = std::vector<LogicType>;
        assert(!functions.empty());
        auto pool(functions);
        LogicTypeCol resultTypes;
        resultTypes.reserve(functions.size());
        std::transform(functions.begin(), functions.end(),
            std::back_inserter(resultTypes),
            [](const Expression &f){return f.result();});
        std::random_device rd;
        std::default_random_engine gen(rd());
        std::uniform_int_distribution<std::size_t> dist(0, functions.size()-1);
        const auto &targetFunc = functions[dist(gen)];
        const auto &args = targetFunc.arguments();
        if(!args.empty())
        {
            std::uniform_int_distribution<std::size_t> argsDist(0, args.size()-1);
            const auto argIdx = argsDist(gen);
            const auto &argType = args[argIdx];
            const auto candidatesCount = std::count(resultTypes.begin(),
                resultTypes.end(), argType);
            if(candidatesCount > 0)
            {
                const auto funcIdx =
                    std::uniform_int_distribution<std::size_t>(0, candidatesCount-1)(gen);
                std::size_t i = 0;
                for(const auto &f : functions)
                {
                    if(f.result() == argType)
                    {
                        if(i == funcIdx)
                        {
                            const auto generatedFunc =
                                targetFunc.dot(argIdx, f);
                            return generatedFunc;
                        }
                        else
                        {
                            ++i;
                        }
                    }
                }
            }
        }
        return targetFunc;
    }
}
