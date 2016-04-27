#ifndef FACT_FILEFACT_H
#define FACT_FILEFACT_H

#include <unordered_map>
#include <string>
#include <vector>

#include "base/IndexAllocator.h"
#include "fact/IFact.h"
#include "fact/Variant.h"

namespace fact
{
    class FileFact: public IFact
    {
    public:
        FileFact(const std::string &filename);

        virtual base::Nullable<String> itemName(ItemId itemId) override;
        virtual base::Nullable<String> propertyName(PropertyId propertyId) override;

        virtual base::Nullable<PropertyCollection> properties(ItemId itemId) override;
        virtual base::Nullable<PropertyCollection> properties(ItemId itemId,
            PropertyId propertyId) override;
        virtual ItemIdCollection find(PropertyId propertyId,
            const Variant &value) override;

        virtual ItemIdCollection items() override;
        
    private:
        using PropertyNameMap = std::unordered_map<PropertyId, String>;
        using ItemNameMap = std::unordered_map<ItemId, String>;
        using PropertyIdMap = std::unordered_map<String, PropertyId>;
        using ItemIdMap = std::unordered_map<String, ItemId>;
        using VariantCol = std::vector<Variant>;
        using PropertyValuesMap = std::unordered_map<PropertyId, VariantCol>;
        using ItemPropertiesMap = std::unordered_map<ItemId,
              PropertyValuesMap>;

    private:
        PropertyId preparePropertyId(const String &property);
        ItemId prepareItemId(const String &item);
        Variant makeVariant(const String &value, const String &type);

    private:
        ItemPropertiesMap itemProperties;
        PropertyNameMap propertyNames;
        ItemNameMap itemNames;
        PropertyIdMap propertyIds;
        ItemIdMap itemIds;

        base::IndexAllocator<PropertyId> propertyIdAllocator;
        base::IndexAllocator<ItemId> itemIdAllocator;
    };
}

#endif
