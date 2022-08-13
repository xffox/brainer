#ifndef CORE_ITASK_H
#define CORE_ITASK_H

#include <string>
#include <cstddef>
#include <optional>

#include "core/String.h"

namespace core
{
    class IRender;

    class ITask
    {
    public:
        struct ValidationResult
        {
            bool valid{};
            std::optional<String> description{};
        };

    public:
        virtual ~ITask(){}

        virtual ValidationResult validate(const String &result) = 0;
        virtual bool done() const = 0;
        virtual String answer() const = 0;
        virtual void describe(IRender &render) const = 0;
        virtual void hint(IRender &render, std::size_t level) const = 0;
    };
}

#endif
