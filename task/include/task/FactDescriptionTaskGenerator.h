#ifndef TASK_FACTDESCRIPTIONTASKGENERATOR_H
#define TASK_FACTDESCRIPTIONTASKGENERATOR_H

#include <memory>
#include <vector>

#include "core/ITaskGenerator.h"
#include "fact/IFact.h"
#include "fact/ItemId.h"

namespace task
{
    class FactDescriptionTaskGenerator: public core::ITaskGenerator
    {
    public:
        FactDescriptionTaskGenerator(std::unique_ptr<fact::IFact> fact, int seed);
        virtual std::unique_ptr<core::ITask> generateTask() override;

    private:
        using ItemIdxCol = std::vector<fact::ItemId>;

    private:
        std::unique_ptr<fact::IFact> fact;
        ItemIdxCol itemIndices;

        static const fact::String DESCRIPTION_PROPERTY;
    };
}

#endif
