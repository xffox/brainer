#ifndef CORE_ITASK_H
#define CORE_ITASK_H

#include <string>
#include <cstddef>

#include "core/String.h"

namespace core
{
    class IRender;

    class ITask
    {
    public:
        virtual ~ITask(){}

        virtual bool validate(const String &result) const = 0;
        virtual String answer() const = 0;
        virtual void describe(IRender &render) const = 0;
        virtual void hint(IRender &render, std::size_t level) const = 0;
    };
}

#endif
