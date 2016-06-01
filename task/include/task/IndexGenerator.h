#ifndef TASK_INDEXGENERATOR_H
#define TASK_INDEXGENERATOR_H

#include <cstddef>
#include <vector>
#include <queue>
#include <unordered_set>

namespace task
{
    // Generate index from a given collection, history length configure after
    // how many indices previously generated index can be repeated.
    class IndexGenerator
    {
    public:
        using IndexSet = std::unordered_set<std::size_t>;

    public:
        IndexGenerator(const IndexSet &indices, std::size_t historyLength,
            int seed);
        std::size_t gen();

    private:
        using IndexCol = std::vector<std::size_t>;
        using IdxQueue = std::queue<std::size_t>;

    private:
        std::size_t historyLength;
        IndexCol indices;
        IdxQueue excludedIndices;
    };
}

#endif
