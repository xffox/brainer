#ifndef CORE_STOPWATCH_H
#define CORE_STOPWATCH_H

#include <sys/time.h>

namespace core
{
    class Stopwatch
    {
    public:
        Stopwatch()
            :prev()
        {
            reset();
        }

        void reset();
        long long elapsed() const;
        
    private:
       timeval prev;
    };
}

#endif
