#include "task/FactTaskGenerator.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <cassert>
#include <unordered_set>
#include <sstream>

#include "fact/String.h"
#include "fact/Item.h"
#include "task/StringSet.h"
#include "task/FactTask.h"

#include "util.h"

namespace task
{
    namespace
    {
        template<typename T>
        std::unique_ptr<T> &notNull(std::unique_ptr<T> &ptr)
        {
            if(ptr.get() == nullptr)
                throw std::invalid_argument("null argument");
            return ptr;
        }

        core::String toString(fact::IFact &fact, const fact::Variant &value)
        {
            if(value.isType<int>())
            {
                const auto v = value.getValue<int>();
                std::wstringstream ss;
                ss<<v;
                return ss.str();
            }
            else if(value.isType<double>())
            {
                const auto v = value.getValue<double>();
                std::wstringstream ss;
                ss<<v;
                return ss.str();
            }
            else if(value.isType<fact::String>())
            {
                const auto v = value.getValue<fact::String>();
                return v;
            }
            else if(value.isType<fact::Item>())
            {
                const auto item = value.getValue<fact::Item>();
                const auto itemName = fact.itemName(item.itemId);
                if(itemName.isNull())
                    throw std::runtime_error(
                        "fact data inconsistent: item name is missing");
                return *itemName;
            }
            throw std::runtime_error("unknown variant type");
        }
    }

    FactTaskGenerator::FactTaskGenerator(unsigned int seed,
        std::unique_ptr<fact::IFact> fact,
        std::unique_ptr<Predicate> propertyFilter)
        :fact(std::move(notNull(fact))),
        propertyFilter(std::move(propertyFilter)), itemIds()
    {
        for(auto itemId : this->fact->items())
        {
            const auto props = this->fact->properties(itemId);
            if(!props.isNull())
            {
                std::size_t count = 0;
                for(const auto &p : *props)
                {
                    const auto propName = this->fact->propertyName(p.propertyId);
                    if(!propName.isNull())
                        if(acceptProperty(*propName))
                            ++count;
                }
                if(count > 0)
                    itemIds.push_back(itemId);
            }
        }
        if(itemIds.empty())
            throw std::runtime_error("fact data is empty");
        srand(seed);
    }

    std::unique_ptr<core::ITask> FactTaskGenerator::generateTask()
    {
        using PropertyIdsSet = std::unordered_set<fact::PropertyId>;
        assert(fact.get());
        assert(!itemIds.empty());
        std::size_t pos = rand()%itemIds.size();
        const auto itemId = itemIds[pos];
        const auto props = fact->properties(itemId);
        if(props.isNull())
            throw std::runtime_error(
                "fact data inconsistent: item is missing");
        assert(!props->empty());
        const auto itemName = fact->itemName(itemId);
        if(itemName.isNull())
            throw std::runtime_error(
                "fact data inconsistent: item name is missing");
        PropertyIdsSet propertyIds;
        std::transform(props->begin(), props->end(),
            std::inserter(propertyIds, propertyIds.begin()),
            [](const fact::IFact::PropertyCollection::value_type &p) {
            return p.propertyId;
            });
        for(auto iter = propertyIds.begin(); iter != propertyIds.end();)
        {
            const auto propName = fact->propertyName(*iter);
            if(propName.isNull() || !acceptProperty(*propName))
                propertyIds.erase(iter++);
            else
                ++iter;
        }
        if(propertyIds.empty())
            throw std::runtime_error(
                "fact data inconsistent");
        const std::size_t offset = rand()%propertyIds.size();
        auto iter = propertyIds.begin();
        std::advance(iter, offset);
        const auto propertyId = *iter;
        const auto propertyName = fact->propertyName(propertyId);
        if(propertyName.isNull())
            throw std::runtime_error(
                "fact data inconsistent: property name is missing");
        StringSet values;
        for(const auto &p : *props)
        {
            if(p.propertyId == propertyId)
                values.insert(toString(*fact, p.value));
        }
        return std::unique_ptr<core::ITask>(new FactTask(
                *itemName, *propertyName, values));
    }

    bool FactTaskGenerator::acceptProperty(const core::String &value) const
    {
        return !propertyFilter.get() || propertyFilter->accept(value);
    }
}
