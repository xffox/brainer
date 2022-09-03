#ifndef CORE_STATS_H
#define CORE_STATS_H

#include <cstddef>
#include <chrono>

namespace core
{
    class Stats
    {
    public:
        Stats &setAnswered(bool answered)
        {
            this->answered = answered;
            return *this;
        }

        Stats &setTimeUs(std::chrono::microseconds time)
        {
            this->time = time;
            return *this;
        }

        Stats &setTries(std::size_t tries)
        {
            this->tries = tries;
            return *this;
        }

    public:
        // TODO: task ids might be required
        bool answered = false;
        std::chrono::microseconds time{};
        std::size_t tries = 0;
    };
}

#endif
