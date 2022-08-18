#ifndef MASTERMIND_BOT_HPP
#define MASTERMIND_BOT_HPP

#include <optional>
#include <unordered_set>
#include <vector>

#include "mastermind/type.hpp"
#include "mastermind/config.hpp"

namespace mastermind
{
    class Bot
    {
    public:
        explicit Bot(const GameConfig &config);

        std::optional<Pattern> step(const Feedback &feedback);

    private:
        using Domain = std::unordered_set<uint>;
        using Candidate = std::vector<Domain>;

    private:
        void incorporateFeedback(const Feedback &feedback);
        bool generatePattern();

        static std::optional<Candidate> match(const Candidate &left,
            const Candidate &right);

        std::optional<Pattern> selectPattern(Pattern &cur) const;

    private:
        GameConfig config;
        Pattern pattern;
        Domain valueDomain;
        std::vector<Candidate> candidates;
    };
}

#endif
