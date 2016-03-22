#ifndef CORE_STATS_H
#define CORE_STATS_H

#include <cstddef>

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

        Stats &setTimeUs(long long timeUs)
        {
            this->timeUs = timeUs;
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
        long long timeUs = 0;
        std::size_t tries = 0;
    };
}

#endif
