#ifndef TASK_STRINGSET_H
#define TASK_STRINGSET_H

#include <unordered_set>

#include "core/String.h"

namespace task
{
    using StringSet = std::unordered_set<core::String>;
}

#endif
