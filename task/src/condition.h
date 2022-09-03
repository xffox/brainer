#ifndef TASK_CONDITION_H
#define TASK_CONDITION_H

#include <type_traits>
#include <stdexcept>

namespace task::condition
{
    template<typename T>
    std::enable_if_t<std::is_convertible_v<T, bool>, T&&>
        notNull(T &&ptr)
    {
        if(!ptr)
        {
            throw std::invalid_argument("null argument");
        }
        return ptr;
    }
}

#endif
