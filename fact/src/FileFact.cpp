#include "fact/FileFact.h"

#include <algorithm>
#include <iterator>
#include <fstream>
#include <stdexcept>
#include <cstddef>
#include <limits>
#include <utility>
#include <sstream>
#include <locale>

#include "csv/csv.h"

#include "fact/Item.h"

namespace fact
{
    // TODO: json storage, use strings to define items and properties?
    FileFact::FileFact(const std::string &filename)
        :propertyIdAllocator(
            std::numeric_limits<PropertyId>::min(),
            std::numeric_limits<PropertyId>::max()),
        itemIdAllocator(
            std::numeric_limits<ItemId>::min(),
            std::numeric_limits<ItemId>::max())
    {
        std::wifstream stream(filename);
        stream.imbue(std::locale(std::locale("")));
        if(!stream.is_open())
            throw std::runtime_error("file open failed");
        csv::Csv<wchar_t> reader(stream, L',', L'"', L'\\');
        const std::size_t COLUMNS = 4;
        try
        {
            while(true)
            {
                const auto row = reader.row();
                if(!row.second)
                    break;
                if(row.first.size() != COLUMNS)
                    throw std::runtime_error("invalid file format");
                const auto &item = row.first[0];
                const auto &property = row.first[1];
                const auto &type = row.first[2];
                const auto &value = row.first[3];
                const auto itemId = prepareItemId(item);
                const auto propertyId = preparePropertyId(property);
                itemProperties[itemId][propertyId].push_back(
                    makeVariant(value, type));
            }
        }
        catch(const std::exception &exc)
        {
            throw;
        }
    }

    base::Nullable<String> FileFact::itemName(ItemId itemId)
    {
        auto iter = itemNames.find(itemId);
        if(iter != itemNames.end())
            return base::Nullable<String>(iter->second);
        return base::Nullable<String>();
    }

    base::Nullable<String> FileFact::propertyName(PropertyId propertyId)
    {
        auto iter = propertyNames.find(propertyId);
        if(iter != propertyNames.end())
            return base::Nullable<String>(iter->second);
        return base::Nullable<String>();
    }

    base::Nullable<IFact::PropertyCollection> FileFact::properties(ItemId itemId)
    {
        auto iter = itemProperties.find(itemId);
        if(iter != itemProperties.end())
        {
            IFact::PropertyCollection result;
            for(const auto &valuesPerProperty : iter->second)
            {
                for(const auto &value : valuesPerProperty.second)
                    result.push_back(Property{valuesPerProperty.first, value});
            }
            return result;
        }
        return base::Nullable<IFact::PropertyCollection>();
    }

    base::Nullable<IFact::PropertyCollection> FileFact::properties(ItemId itemId,
            PropertyId propertyId)
    {
        auto iter = itemProperties.find(itemId);
        if(iter != itemProperties.end())
        {
            auto valuesIter = iter->second.find(propertyId);
            if(valuesIter != iter->second.end())
            {
                IFact::PropertyCollection result;
                for(const auto &value : valuesIter->second)
                    result.push_back(Property{valuesIter->first, value});
                return result;
            }
        }
        return base::Nullable<IFact::PropertyCollection>();
    }

    IFact::ItemIdCollection FileFact::find(PropertyId propertyId,
        const Variant &value)
    {
        ItemIdCollection result;
        for(const auto &props : itemProperties)
        {
            auto iter = props.second.find(propertyId);
            for(const auto &propValue : iter->second)
            {
                if(propValue == value)
                {
                    result.push_back(props.first);
                    break;
                }
            }
        }
        return result;
    }

    PropertyId FileFact::preparePropertyId(const String &property)
    {
        auto propertyIter = propertyIds.find(property);
        if(propertyIter == propertyIds.end())
        {
            const auto propertyId = propertyIdAllocator.alloc();
            propertyIter = propertyIds.insert(
                std::make_pair(property, propertyId)).first;
            propertyNames.insert(
                std::make_pair(propertyId, property));
        }
        return propertyIter->second;
    }

    ItemId FileFact::prepareItemId(const String &item)
    {
        auto itemIter = itemIds.find(item);
        if(itemIter == itemIds.end())
        {
            const auto itemId = itemIdAllocator.alloc();
            itemIter = itemIds.insert(
                std::make_pair(item, itemId)).first;
            itemNames.insert(std::make_pair(itemId, item));
        }
        return itemIter->second;
    }

    Variant FileFact::makeVariant(const String &value, const String &type)
    {
        if(type == L"string")
        {
            return Variant(value);
        }
        else if(type == L"integer")
        {
            // TODO: safe convertion
            std::wstringstream ss(value);
            int v = 0;
            ss>>v;
            return Variant(value);
        }
        else if(type == L"float")
        {
            // TODO: safe convertion
            std::wstringstream ss(value);
            double v = 0;
            ss>>v;
            return Variant(value);
        }
        else if(type == L"item")
        {
            const auto itemId = prepareItemId(value);
            return Variant(Item{itemId});
        }
        throw std::runtime_error("unknown property value type");
    }

    FileFact::ItemIdCollection FileFact::items()
    {
        ItemIdCollection result;
        std::transform(itemProperties.begin(), itemProperties.end(), std::back_inserter(result),
            [](const ItemPropertiesMap::value_type &p){return p.first;});
        return result;
    }
}
