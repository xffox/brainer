#ifndef FACT_ITEM_H
#define FACT_ITEM_H

#include "fact/ItemId.h"

namespace fact
{
    struct Item
    {
        ItemId itemId;
    };
}

inline bool operator==(const fact::Item &left, const fact::Item &right)
{
    return left.itemId == right.itemId;
}

#endif
