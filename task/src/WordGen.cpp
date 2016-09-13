#include "task/WordGen.h"

#include <cassert>

namespace task
{
    WordGen::WordGen(const StringCol &words)
        :transitions(), eowProbs(), lengthes()
    {
        using CountMap = std::unordered_map<core::String::value_type, double>;
        CountMap counts;
        double wordCount = 0.0;
        for(const auto &w : words)
        {
            if(w.empty())
                continue;
            wordCount += 1.0;
            for(std::size_t i = 0; i+1 < w.size(); ++i)
            {
                transitions[w[i]][w[i+1]] += 1.0;
                counts[w[i]] += 1.0;
            }
            // TODO: make sure sums to 1.0
            transitions[EOW][w.front()] += 1.0;
            counts[EOW] += 1.0;
            eowProbs[w.back()] += 1.0;
            lengthes[w.size()] += 1.0;
        }
        for(auto &probs : transitions)
        {
            auto iter = counts.find(probs.first);
            assert(iter != counts.end());
            const auto count = iter->second;
            for(auto &p : probs.second)
                p.second /= count;
        }
        for(auto &p : eowProbs)
            p.second /= wordCount;
        for(auto &p : lengthes)
            p.second /= wordCount;
    }

    core::String WordGen::generate(RandomEngine &gen) const
    {
        core::String res;
        core::String::value_type cur = EOW;
        std::uniform_real_distribution<> dist(0.0, 1.0);
        std::size_t len = 0;
        double eowCumsum = 0.0;
        while(true)
        {
            auto lengthesIter = lengthes.find(len);
            if(lengthesIter != lengthes.end())
                eowCumsum += lengthesIter->second;
            const auto eowRand = dist(gen);
            auto eowProbsIter = eowProbs.find(cur);
            if(eowProbsIter != eowProbs.end() && eowProbsIter->second > 0.0)
            {
                if(eowRand < eowCumsum)
                    break;
            }
            auto iter = transitions.find(cur);
            assert(iter != transitions.end());
            auto s = 0.0;
            bool found = false;
            core::String::value_type last = EOW;
            const auto transRand = dist(gen);
            for(const auto &p : iter->second)
            {
                s += p.second;
                if(transRand < s)
                {
                    cur = p.first;
                    found = true;
                    break;
                }
                last = p.first;
            }
            if(!found)
                cur = last;
            if(cur == EOW)
                break;
            res.push_back(cur);
            len += 1;
        }
        return res;
    }
}
