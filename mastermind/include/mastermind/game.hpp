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
    public:
        static std::optional<Game> create(const GameConfig &config);
        static std::optional<Game> create(const GameConfig &config,
            std::mt19937::result_type seed);

        Session startRandom();
        std::optional<Session> start(const Pattern &pattern) const;

    private:
        Game(const GameConfig &config, std::mt19937::result_type seed);

    private:
        GameConfig config;
        std::mt19937 rand;
    };
}

#endif
