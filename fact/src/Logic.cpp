#include "fact/Logic.h"

#include <algorithm>
#include <vector>
#include <cstddef>

namespace fact
{
    // TODO: optimal search in case of missing ordering of a type
    LogicFunc Logic::generate() const
    {
        using IdxCol = std::vector<std::size_t>;
        auto pool(functions);
        IdxCol indices;
    }
}
