#ifndef MASTERMIND_CONFIG_HPP
#define MASTERMIND_CONFIG_HPP

#include "mastermind/type.hpp"

namespace mastermind
{
    struct GameConfig
    {
        uint colors = 10;
        uint length = 4;
        uint guesses = 8;
        bool repeats = true;
    };

}

#endif
