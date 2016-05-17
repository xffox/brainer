#ifndef TASK_CONDITION_H
#define TASK_CONDITION_H

#include <memory>
#include <stdexcept>

namespace task
{
    namespace condition
    {
        template<typename T>
        std::unique_ptr<T> &notNull(std::unique_ptr<T> &ptr)
        {
            if(ptr.get() == nullptr)
                throw std::invalid_argument("null argument");
            return ptr;
        }

    }
}

#endif
