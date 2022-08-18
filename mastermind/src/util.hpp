#ifndef MASTERMIND_UTIL_HPP
#define MASTERMIND_UTIL_HPP

#include "mastermind/type.hpp"
#include "mastermind/config.hpp"

namespace mastermind::util
{
    bool validPattern(const Pattern &pattern, const GameConfig &config);
    Feedback matchGuess(const Pattern &pattern, const Pattern &guess);
}

#endif
