#ifndef CORE_ITASK_H
#define CORE_ITASK_H

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
        virtual ~ITask() = default;

        virtual ValidationResult validate(const String &result) = 0;
        [[nodiscard]]
        virtual bool done() const = 0;
        [[nodiscard]]
        virtual String answer() const = 0;
        virtual void describe(IRender &render) const = 0;
        virtual void hint(IRender &render, std::size_t level) = 0;
    };
}

#endif
