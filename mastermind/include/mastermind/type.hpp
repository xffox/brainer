#ifndef MASTERMIND_TYPE_HPP
#define MASTERMIND_TYPE_HPP

#include <vector>

namespace mastermind
{
    using uint = unsigned int;
    using Pattern = std::vector<uint>;

    struct Feedback
    {
        uint white;
        uint black;
    };
}

#endif
