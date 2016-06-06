#ifndef TASK_FACTDESCRIPTIONTASKGENERATOR_H
#define TASK_FACTDESCRIPTIONTASKGENERATOR_H

#include <memory>
#include <vector>

#include "core/ITaskGenerator.h"
#include "fact/IFact.h"
#include "fact/ItemId.h"
#include "task/IndexGenerator.h"

namespace task
{
    class FactDescriptionTaskGenerator: public core::ITaskGenerator
    {
    public:
        FactDescriptionTaskGenerator(std::unique_ptr<fact::IFact> fact, int seed);
        virtual std::unique_ptr<core::ITask> generateTask() override;

    private:
        struct InitializedIndexGenerator
        {
            InitializedIndexGenerator(IndexGenerator::IndexSet &&indices,
                int seed)
                :generator(indices, indices.size()/2, seed)
            {}

            IndexGenerator generator;
        };

    private:
        FactDescriptionTaskGenerator(IndexGenerator::IndexSet &&indices,
            std::unique_ptr<fact::IFact> fact, int seed);

        static IndexGenerator::IndexSet prepareIndices(fact::IFact &fact);

    private:
        static const fact::String DESCRIPTION_PROPERTY;

    private:
        std::unique_ptr<fact::IFact> fact;
        InitializedIndexGenerator indexGenerator;
    };
}

#endif
