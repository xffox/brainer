#ifndef TASK_UTIL_H
#define TASK_UTIL_H

#include <algorithm>
#include <iterator>
#include <string>

#include "core/String.h"

namespace task
{
    namespace util
    {
        bool toRegularStr(std::string &result, const core::String &str);
        core::String toWString(const std::string &str);

        core::String tolower(const core::String &str);
        template<class InputIter, class OutputIter>
        void toLowerCollection(InputIter begin, InputIter end, OutputIter out)
        {
            std::transform(begin, end, out,
                [](const core::String &str) {
                    return tolower(str);
                });
        }
    }
}

#endif
