#include "Stopwatch.h"

namespace core
{
    // TODO: test
    namespace
    {
        long long diff(const timeval &left, const timeval &right)
        {
            long long result = left.tv_usec;
            time_t sm = 0;
            if(result < right.tv_usec)
            {
                result += 1000000;
                sm = 1;
            }
            result -= right.tv_usec;
            result += (left.tv_sec - right.tv_sec - sm)*1000000;
            return result;
        }
    }

    void Stopwatch::reset()
    {
        gettimeofday(&prev, 0);
    }

    long long Stopwatch::elapsed() const
    {
        timeval current;
        gettimeofday(&current, 0);
        return diff(current, prev);
    }
}
