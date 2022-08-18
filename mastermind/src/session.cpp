#include "mastermind/session.hpp"

#include "mastermind/type.hpp"
#include "util.hpp"

namespace mastermind
{
    Session::Session(const Pattern &pattern, const GameConfig &config)
        :pattern_(pattern), config(config), leftGuesses(config.guesses)
    {}

    std::optional<Feedback> Session::step(const Pattern &guess)
    {
        if(!util::validPattern(guess, config))
        {
            return {};
        }
        if(leftGuesses == 0)
        {
            return {};
        }
        auto feedback = util::matchGuess(pattern_, guess);
        if(feedback.white == config.length)
        {
            leftGuesses = 0;
            cracked_ = true;
        }
        else
        {
            --leftGuesses;
        }
        return feedback;
    }

    SessionState Session::state() const
    {
        if(leftGuesses > 0)
        {
            return SessionState::PLAYING;
        }
        return cracked_ ? SessionState::WON : SessionState::LOST;
    }
}
