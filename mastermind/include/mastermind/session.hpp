#ifndef MASTERMIND_SESSION_HPP
#define MASTERMIND_SESSION_HPP

#include <optional>

#include "mastermind/type.hpp"
#include "mastermind/config.hpp"

namespace mastermind
{
    enum class SessionState
    {
        PLAYING,
        WON,
        LOST,
    };

    class Session
    {
        friend class Game;
    public:
        std::optional<Feedback> step(const Pattern &guess);

        [[nodiscard]]
        SessionState state() const;

        [[nodiscard]]
        const Pattern &pattern() const
        {
            return pattern_;
        }

        [[nodiscard]]
        uint guessIndex() const
        {
            return config.guesses - leftGuesses;
        }

    private:
        explicit Session(Pattern pattern, const GameConfig &config);

    private:
        Pattern pattern_;
        GameConfig config;
        uint leftGuesses;
        bool cracked_ = false;
    };
}

#endif
