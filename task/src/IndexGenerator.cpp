#include "task/IndexGenerator.h"

#include <cstdlib>
#include <stdexcept>
#include <cassert>

namespace task
{
    IndexGenerator::IndexGenerator(const IndexSet &indices,
        std::size_t historyLength, int seed)
        :historyLength(historyLength), indices(indices.begin(), indices.end()),
        excludedIndices()
    {
        if(indices.empty())
            throw std::invalid_argument(
                "empty indices");
        if(historyLength >= indices.size())
            throw std::invalid_argument(
                "history length bigger than indices size");
        srand(seed);
    }

    std::size_t IndexGenerator::gen()
    {
        assert(!indices.empty());
        std::size_t pos = rand()%indices.size();
        const auto idx = indices[pos];
        if(historyLength > 0)
        {
            if(excludedIndices.size() < historyLength)
            {
                indices.erase(indices.begin()+pos);
            }
            else
            {
                const auto excludedIdx = excludedIndices.front();
                excludedIndices.pop();
                indices[pos] = excludedIdx;
            }
            excludedIndices.push(idx);
        }
        return idx;
    }
}
