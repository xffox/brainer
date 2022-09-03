#include "task/IndexGenerator.h"

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <cassert>

namespace task
{
    IndexGenerator::IndexGenerator(base::Randomizer &&random,
        const IndexSet &indices, std::size_t historyLength)
        :historyLength(historyLength), indices(indices.begin(), indices.end()),
        excludedIndices(), random(std::move(random))
    {
        if(indices.empty())
        {
            throw std::invalid_argument(
                "empty indices");
        }
        if(historyLength >= indices.size())
        {
            throw std::invalid_argument(
                "history length bigger than indices size");
        }
    }

    std::size_t IndexGenerator::gen()
    {
        assert(!indices.empty());
        auto pos = random.uniformInteger<std::size_t>(0, indices.size()-1);
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
