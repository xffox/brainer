#ifndef TASK_TAGAINI_H
#define TASK_TAGAINI_H

#include <vector>
#include <utility>
#include <istream>

#include "core/String.h"
#include "task/StringCollection.h"

namespace task
{
    namespace tagaini
    {
        using TaskCollection = std::vector<std::pair<core::String, StringCollection>>;

        TaskCollection readCollection(std::wistream &stream);
    }
}

#endif
