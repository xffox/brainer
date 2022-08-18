#include "mastermind/bot.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <cassert>

#include "mastermind/type.hpp"

namespace mastermind
{
    namespace
    {
        template<typename F>
        void generateExact(std::vector<uint> &indices,
            uint values, uint size,
            F func)
        {
            if(indices.size() == size)
            {
                func(indices);
                return;
            }
            for(uint i = (indices.empty() ? 0 : indices.back()+1);
                i+(size-1-indices.size()) < values; ++i)
            {
                indices.push_back(i);
                generateExact(indices, values, size, func);
                indices.pop_back();
            }
        }

        template<typename F>
        void generateSame(std::unordered_map<uint, uint> &tr,
            std::unordered_set<uint> &occupied, uint cur,
            const std::unordered_set<uint> &used,
            const Pattern &pattern,
            uint values, uint size,
            F func)
        {
            assert(pattern.size() == values);
            if(tr.size() == size)
            {
                func(tr);
                return;
            }
            for(uint i = cur;
                i+(size-1-tr.size()) < values; ++i)
            {
                if(used.find(i) == std::end(used))
                {
                    for(uint j = 0; j < values; ++j)
                    {
                        if(occupied.insert(j).second)
                        {
                            if(pattern[i] != pattern[j])
                            {
                                tr[i] = j;
                                generateSame(tr, occupied, i+1, used,
                                    pattern, values, size, func);
                                tr.erase(i);
                            }
                            occupied.erase(j);
                        }
                    }
                }
            }
        }

        auto prepareValueDomain(uint values)
        {
            std::unordered_set<uint> res;
            std::generate_n(std::inserter(res, std::end(res)), values,
                [n=static_cast<uint>(0)]() mutable {
                    return n++;
                });
            return res;
        }
    }

    Bot::Bot(const GameConfig &config)
        :config(config), pattern(),
        valueDomain(prepareValueDomain(config.colors)),
        candidates{Candidate(config.length, valueDomain)}
    {}

    std::optional<Pattern> Bot::step(const Feedback &feedback)
    {
        incorporateFeedback(feedback);
        if(!generatePattern())
        {
            return {};
        }
        return pattern;
    }

    void Bot::incorporateFeedback(const Feedback &feedback)
    {
        if(pattern.empty())
        {
            return;
        }
        std::vector<Candidate> newCandidates;
        std::vector<uint> exacts;
        generateExact(exacts, config.length, feedback.white,
            [&newCandidates, &feedback, this](const auto &indices){
                std::unordered_set<uint> occupied(
                    std::begin(indices), std::end(indices));
                std::unordered_map<uint, uint> tr;
                generateSame(tr, occupied, 0,
                    std::unordered_set<uint>(occupied),
                    pattern, config.length, feedback.black,
                    [&newCandidates, &indices, this](const auto &tr){
                        Candidate cand(pattern.size(), valueDomain);
                        auto unusedPattern = prepareValueDomain(config.length);
                        auto unusedCand = unusedPattern;
                        for(auto idx : indices)
                        {
                            cand[idx] = Domain{pattern[idx]};
                            unusedPattern.erase(idx);
                            unusedCand.erase(idx);
                        }
                        for(const auto &t : tr)
                        {
                            cand[t.second] = Domain{pattern[t.first]};
                            unusedPattern.erase(t.first);
                            unusedCand.erase(t.second);
                        }
                        for(auto patternIdx : unusedPattern)
                        {
                            for(auto candIdx : unusedCand)
                            {
                                cand[candIdx].erase(pattern[patternIdx]);
                            }
                        }
                        for(const auto &prevCand : candidates)
                        {
                            auto res = match(prevCand, cand);
                            if(res)
                            {
                                newCandidates.push_back(std::move(*res));
                            }
                        }
                    });
            });
        candidates = std::move(newCandidates);
    }

    bool Bot::generatePattern()
    {
        Pattern selectedPattern;
        auto res =
            selectPattern(selectedPattern);
        if(res)
        {
            pattern = *res;
        }
        return static_cast<bool>(res);
    }

    std::optional<Bot::Candidate> Bot::match(const Candidate &left,
        const Candidate &right)
    {
        assert(left.size() == right.size());
        Candidate res;
        res.reserve(left.size());
        for(uint i = 0; i < left.size(); ++i)
        {
            const auto &r = right[i];
            res.emplace_back();
            for(auto v : left[i])
            {
                if(r.find(v) != std::end(r))
                {
                    res.back().insert(v);
                }
            }
            if(res.back().empty())
            {
                return {};
            }
        }
        return res;
    }

    std::optional<Pattern> Bot::selectPattern(Pattern &cur) const
    {
        if(cur.size() == config.length)
        {
            return cur;
        }
        std::vector<uint> sortedValues;
        const auto idx = cur.size();
        std::unordered_map<uint, int> counts;
        for(const auto &cand : candidates)
        {
            bool valid = true;
            for(uint i = 0; i < cur.size(); ++i)
            {
                if(cand[i].find(cur[i]) == std::end(cand[i]))
                {
                    valid = false;
                    break;
                }
            }
            if(valid)
            {
                for(auto v : cand[idx])
                {
                    counts[v] += 1;
                }
            }
        }
        if(counts.empty())
        {
            return {};
        }
        std::vector<uint> values;
        values.reserve(counts.size());
        std::transform(std::begin(counts), std::end(counts),
            std::back_inserter(values),
            [](const auto &p){
                return p.first;
            });
        const int half = candidates.size()/2;
        std::sort(std::begin(values), std::end(values),
            [&counts, half](auto left, auto right){
            return std::abs(counts.at(left) - half) <
                std::abs(counts.at(right) - half);
            });
        for(auto v : values)
        {
            if(!config.repeats &&
                std::find(std::begin(cur), std::end(cur), v) != std::end(cur))
            {
                continue;
            }
            cur.push_back(v);
            auto res = selectPattern(cur);
            if(res)
            {
                return res;
            }
            cur.pop_back();
        }
        return {};
    }
}
