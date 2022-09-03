#ifndef CORE_STOPWATCH_H
#define CORE_STOPWATCH_H

#include <chrono>

namespace core
{
    class Stopwatch
    {
    public:
        Stopwatch();

        void reset();
        [[nodiscard]]
        std::chrono::microseconds elapsed() const;

    private:
        using Watch = std::chrono::steady_clock;

    private:
        Watch::time_point prev;
    };
}

#endif
