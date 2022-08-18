#ifndef TASK_MASTERMINDTASKGENERATOR_H
#define TASK_MASTERMINDTASKGENERATOR_H

#include "core/ITaskGenerator.h"
#include <mastermind/game.hpp>

namespace task
{
    class MastermindTaskGenerator: public core::ITaskGenerator
    {
    public:
        MastermindTaskGenerator();

        std::unique_ptr<core::ITask> generateTask() override;

    private:
        mastermind::Game game;
    };
}

#endif
