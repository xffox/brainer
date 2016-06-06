#include "task/FactDescriptionTaskGenerator.h"

#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <cassert>

#include "task/FactDescriptionTask.h"

#include "condition.h"

namespace task
{
    namespace
    {
        template<typename TargetIterator, typename PatternIterator>
        inline TargetIterator searchIgnoringCase(TargetIterator targetBegin,
            TargetIterator targetEnd, PatternIterator patternBegin,
            PatternIterator patternEnd)
        {
            return std::search(targetBegin, targetEnd, patternBegin,
                patternEnd,
                [](const core::String::value_type &left,
                    const core::String::value_type &right) {
                    return std::tolower(left) == std::tolower(right);
                });
        }

        core::String replaceIgnoringCase(const core::String &str,
            const core::String &pattern, const core::String &replacement)
        {
            core::String result;
            auto cur = str.begin();
            while(cur != str.end())
            {
                auto patternIter = searchIgnoringCase(cur, str.end(),
                    pattern.begin(), pattern.end());
                result.insert(result.end(), cur, patternIter);
                if(patternIter != str.end())
                {
                    result.insert(result.end(), replacement.begin(), replacement.end());
                    cur = patternIter + pattern.size();
                }
                else
                {
                    cur = patternIter;
                }
            }
            return result;
        }
    }

    FactDescriptionTaskGenerator::FactDescriptionTaskGenerator(
        std::unique_ptr<fact::IFact> fact, int seed)
        :fact(std::move(condition::notNull(fact))),
        indexGenerator(prepareIndices(*this->fact), seed)
    {}

    std::unique_ptr<core::ITask> FactDescriptionTaskGenerator::generateTask()
    {
        const auto itemId = indexGenerator.generator.gen();
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
                return std::unique_ptr<core::ITask>(
                    new FactDescriptionTask(*itemName,
                        replaceIgnoringCase(
                            prop.value.getValue<fact::String>(),
                            *itemName, L"?")));
            }
        }
        throw std::runtime_error("inconsistent facts data");
    }

    IndexGenerator::IndexSet FactDescriptionTaskGenerator::prepareIndices(fact::IFact &fact)
    {
        IndexGenerator::IndexSet itemIndices;
        for(const auto itemId : fact.items())
        {
            const auto properties = fact.properties(itemId);
            if(properties.isNull())
            {
                // TODO: proper exception
                throw std::runtime_error("inconsistent facts data");
            }
            const auto itemName = fact.itemName(itemId);
            if(itemName.isNull())
            {
                throw std::runtime_error("inconsistent facts data");
            }
            for(const auto &prop : *properties)
            {
                const auto propName = fact.propertyName(
                    prop.propertyId);
                if(!propName.isNull() && *propName == DESCRIPTION_PROPERTY)
                {
                    const auto &value = prop.value.getValue<core::String>();
                    if(searchIgnoringCase(value.begin(), value.end(),
                            itemName->begin(), itemName->end()) != value.end())
                    {
                        itemIndices.insert(itemId);
                        break;
                    }
                }
            }
        }
        if(itemIndices.empty())
            throw std::runtime_error("no items with description");
        return itemIndices;
    }

    const fact::String FactDescriptionTaskGenerator::DESCRIPTION_PROPERTY = L"description";
}
