#include "task/FactDescriptionTaskGenerator.h"

#include <stdexcept>
#include <cstdlib>
#include <cassert>

#include "task/FactDescriptionTask.h"

#include "condition.h"

namespace task
{
    FactDescriptionTaskGenerator::FactDescriptionTaskGenerator(std::unique_ptr<fact::IFact> fact,
        int seed)
        :fact(std::move(condition::notNull(fact))), itemIndices()
    {
        srand(seed);
        for(const auto itemId : this->fact->items())
        {
            const auto properties = this->fact->properties(itemId);
            if(properties.isNull())
            {
                // TODO: proper exception
                throw std::runtime_error("inconsistent facts data");
            }
            for(const auto &prop : *properties)
            {
                const auto propName = this->fact->propertyName(prop.propertyId);
                if(!propName.isNull() && *propName == DESCRIPTION_PROPERTY)
                {
                    itemIndices.push_back(itemId);
                    break;
                }
            }
        }
        if(itemIndices.empty())
            throw std::runtime_error("no items with description");
    }

    std::unique_ptr<core::ITask> FactDescriptionTaskGenerator::generateTask()
    {
        assert(!itemIndices.empty());
        const auto itemId = itemIndices[rand()%itemIndices.size()];
        const auto itemName = fact->itemName(itemId);
        if(itemName.isNull())
        {
            throw std::runtime_error("inconsistent facts data");
        }
        const auto properties = this->fact->properties(itemId);
        if(properties.isNull())
        {
            // TODO: proper exception
            throw std::runtime_error("inconsistent facts data");
        }
        for(const auto &prop : *properties)
        {
            const auto propName = this->fact->propertyName(prop.propertyId);
            if(!propName.isNull() && *propName == DESCRIPTION_PROPERTY)
            {
                itemIndices.push_back(itemId);
                return std::unique_ptr<core::ITask>(
                    new FactDescriptionTask(*itemName, prop.value.getValue<fact::String>()));
            }
        }
        throw std::runtime_error("inconsistent facts data");
    }

    const fact::String FactDescriptionTaskGenerator::DESCRIPTION_PROPERTY = L"description";
}
