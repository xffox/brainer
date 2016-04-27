#ifndef FACT_IFACT_H
#define FACT_IFACT_H

#include <vector>

#include "base/Nullable.h"
#include "fact/PropertyId.h"
#include "fact/ItemId.h"
#include "fact/String.h"
#include "fact/Property.h"

namespace fact
{
    class IFact
    {
    public:
        using PropertyCollection = std::vector<Property>;
        using ItemIdCollection = std::vector<ItemId>;

    public:
        virtual ~IFact(){}

        virtual base::Nullable<String> itemName(ItemId itemId) = 0;
        virtual base::Nullable<String> propertyName(PropertyId propertyId) = 0;

        virtual base::Nullable<PropertyCollection> properties(ItemId itemId) = 0;
        virtual base::Nullable<PropertyCollection> properties(ItemId itemId,
            PropertyId propertyId) = 0;
        virtual ItemIdCollection find(PropertyId propertyId,
            const Variant &value) = 0;

        virtual ItemIdCollection items() = 0;
    };
}

#endif
