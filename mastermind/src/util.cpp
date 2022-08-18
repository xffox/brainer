#include "util.hpp"

#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <unordered_map>
#include <cassert>

#include "mastermind/type.hpp"

namespace mastermind::util
{
    namespace
    {
        bool haveRepeats(const Pattern &pattern)
        {
            return std::unordered_set(
                std::begin(pattern), std::end(pattern)).size() !=
                pattern.size();
        }

        bool allInDomain(const Pattern &pattern, uint domainSize)
        {
            return std::all_of(std::begin(pattern), std::end(pattern),
                [domainSize](auto v){
                    return v < domainSize;
                });
        }
    }

    bool validPattern(const Pattern &pattern, const GameConfig &config)
    {
        return (pattern.size() == config.length) &&
            allInDomain(pattern, config.colors) &&
            (config.repeats || !haveRepeats(pattern));
    }

    Feedback matchGuess(const Pattern &pattern, const Pattern &guess)
    {
        assert(pattern.size() == guess.size());
        std::unordered_map<uint, uint> unused{};
        std::unordered_map<uint, uint> unmatched{};
        Feedback feedback{};
        for(uint i = 0; i < pattern.size(); ++i)
        {
            if(pattern[i] == guess[i])
            {
                ++feedback.white;
            }
            else
            {
                unmatched[pattern[i]] += 1;
                unused[guess[i]] += 1;
            }
        }
        for(const auto &p : unused)
        {
            auto iter = unmatched.find(p.first);
            if(iter != std::end(unmatched))
            {
                feedback.black += std::min(p.second, iter->second);
            }
        }
        return feedback;
    }
}
