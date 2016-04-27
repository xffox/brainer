#ifndef TASK_FACTTASKGENERATOR_H
#define TASK_FACTTASKGENERATOR_H

#include <memory>

#include "core/ITaskGenerator.h"
#include "fact/IFact.h"
#include "core/String.h"

namespace task
{
    class FactTaskGenerator: public core::ITaskGenerator
    {
    public:
        class Predicate
        {
        public:
            virtual ~Predicate(){}
            virtual bool accept(const core::String &value) const = 0;
        };

    public:
        FactTaskGenerator(unsigned int seed, std::unique_ptr<fact::IFact> fact,
            std::unique_ptr<Predicate> propertyFilter =
                std::unique_ptr<Predicate>());

        virtual std::unique_ptr<core::ITask> generateTask() override;

    private:
        bool acceptProperty(const core::String &value) const;

    private:
        std::unique_ptr<fact::IFact> fact;
        std::unique_ptr<Predicate> propertyFilter;
        fact::IFact::ItemIdCollection itemIds;
    };
}

#endif
