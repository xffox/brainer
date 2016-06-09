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
    // TODO: optimal search in case of missing ordering of a type
    Expression Logic::generate(const ExpressionCol &functions) const
    {
        if(functions.empty())
            throw std::invalid_argument("empty functions list");
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

    Logic::ExpressionCol Logic::generate(const ExpressionCol &functions,
        const LogicType::ParameterCol &availableParams) const
    {
        std::random_device rd;
        std::default_random_engine gen(rd());
        ExpressionCol result;
        ExpressionCol pool;
        ExpressionCol allFuncs(functions);
        if(functions.empty())
            return result;
        for(const auto &f : functions)
        {
            bool available = true;
            for(const auto &p : f.arguments())
            {
                auto iter = std::find(availableParams.begin(), availableParams.end(), p);
                if(iter == availableParams.end())
                {
                    available = false;
                    break;
                }
            }
            if(available)
                pool.push_back(f);
        }
        if(pool.empty())
            return result;
        const std::size_t ITERS = 40;
        for(std::size_t it = 0; it < ITERS;)
        {
            std::uniform_int_distribution<std::size_t> dist(0, allFuncs.size()-1);
            auto f = allFuncs[dist(gen)];
            bool valid = true;
            const auto targetParams = f.arguments();
            if(!targetParams.empty())
            {
                for(std::size_t i = targetParams.size(); i > 0; --i)
                {
                    const auto &targetParam = targetParams[i-1];
                    auto iter = std::find(availableParams.begin(), availableParams.end(), targetParam);
                    if(iter != availableParams.end())
                        continue;
                    ExpressionCol candidateExpressions;
                    for(const auto &candExpr : pool)
                    {
                        if(candExpr.result() == targetParam)
                            candidateExpressions.push_back(candExpr);
                    }
                    if(candidateExpressions.empty())
                    {
                        valid = false;
                        break;
                    }
                    const auto &selectedCandidate = candidateExpressions[
                        std::uniform_int_distribution<std::size_t>(0, candidateExpressions.size()-1)(gen)];
                    f = f.dot(i-1, selectedCandidate);
                }
            }
            if(valid)
            {
                auto iter = std::find(result.begin(), result.end(), f);
                if(iter == result.end())
                {
                    result.push_back(f);
                    pool.push_back(f);
                    allFuncs.push_back(f);
                }
            }
            ++it;
        }
        return result;
    }
}
