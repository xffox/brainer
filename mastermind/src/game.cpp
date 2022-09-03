#include "mastermind/game.hpp"

#include <algorithm>
#include <iterator>
#include <random>
#include <utility>

#include "util.hpp"

namespace mastermind
{
    namespace
    {
        bool validConfig(const GameConfig &config)
        {
            return config.colors > 0 &&
                config.length > 0 &&
                config.guesses > 0 &&
                (config.repeats || config.colors >= config.length);
        }
    }

    Game::Game(const GameConfig &config, Seed seed)
        :config(config), rand(seed)
    {}

    std::optional<Game> Game::create(const GameConfig &config)
    {
        return create(config, std::random_device()());
    }

    std::optional<Game> Game::create(const GameConfig &config, Seed seed)
    {
        if(!validConfig(config))
        {
            return {};
        }
        return Game(config, seed);
    }

    Session Game::startRandom()
    {
        Pattern pattern;
        if(config.repeats)
        {
            pattern.reserve(config.length);
            std::uniform_int_distribution<uint> dist(0, config.colors-1);
            for(uint i = 0; i < config.length; ++i)
            {
                pattern.push_back(dist(rand));
            }
        }
        else
        {
            pattern.reserve(config.colors);
            std::generate_n(std::back_inserter(pattern), config.colors,
                [n=static_cast<uint>(0)]() mutable {
                    return n++;
                });
            std::shuffle(std::begin(pattern), std::end(pattern), rand);
            pattern.resize(config.length);
        }
        return Session(std::move(pattern), config);
    }

    std::optional<Session> Game::start(const Pattern &pattern) const
    {
        if(!util::validPattern(pattern, config))
        {
            return {};
        }
        return Session(pattern, config);
    }
}
