#ifndef BASE_INDEXALLOCATOR_H
#define BASE_INDEXALLOCATOR_H

#include <set>
#include <stdexcept>

namespace base
{
    template<typename Index>
    class IndexAllocator
    {
    public:
        IndexAllocator(Index first, Index last);

        // throw std::exception when no more indexes available
        Index alloc();
        void release(Index index);

    private:
        using IndexesCollection = std::set<Index>;

    private:
        Index max;
        IndexesCollection indexes;
        Index first;
        Index last;
    };

    template<typename Index>
    IndexAllocator<Index>::IndexAllocator(Index first, Index last)
        :max(first), first(first), last(last)
    {
        indexes.insert(max);
    }

    template<typename Index>
    Index IndexAllocator<Index>::alloc()
    {
        if(!indexes.empty())
        {
            const Index index = *indexes.begin();
            indexes.erase(indexes.begin());
            return index;
        }
        if(max < last)
        {
            return ++max;
        }
        throw std::length_error("all indices allocated");
    }

    template<typename Index>
    void IndexAllocator<Index>::release(Index index)
    {
        if(index >= first && index <= max &&
            indexes.find(index) == indexes.end())
        {
            indexes.insert(index);
        }
        else
        {
            throw std::invalid_argument("invalid index");
        }
    }
}

#endif
