#ifndef MASTERMIND_GAME_HPP
#define MASTERMIND_GAME_HPP

#include <optional>
#include <random>

#include "mastermind/type.hpp"
#include "mastermind/config.hpp"
#include "mastermind/session.hpp"

namespace mastermind
{
    class Game
    {
        using Random = std::mt19937;
    public:
        using Seed = Random::result_type;

    public:
        static std::optional<Game> create(const GameConfig &config);
        static std::optional<Game> create(const GameConfig &config,
            Seed seed);

        Session startRandom();
        [[nodiscard]]
        std::optional<Session> start(const Pattern &pattern) const;

    private:
        Game(const GameConfig &config, Seed seed);

    private:
        GameConfig config;
        Random rand;
    };
}

#endif
