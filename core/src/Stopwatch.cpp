#include "Stopwatch.h"

namespace core
{
    Stopwatch::Stopwatch()
        :prev()
    {
        reset();
    }

    void Stopwatch::reset()
    {
        prev = Watch::now();
    }

    std::chrono::microseconds Stopwatch::elapsed() const
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(
            Watch::now()-prev);
    }
}
