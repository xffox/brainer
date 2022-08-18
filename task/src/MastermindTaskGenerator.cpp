#include "task/MastermindTaskGenerator.h"

#include <cassert>

#include "task/MastermindTask.h"
#include "mastermind/config.hpp"

namespace task
{
    namespace
    {
        constexpr mastermind::GameConfig CONFIG{
            6, 4, 8, true
        };

        mastermind::Game prepareGame()
        {
            auto res = mastermind::Game::create(CONFIG);
            assert(res);
            return *res;
        }
    }

    MastermindTaskGenerator::MastermindTaskGenerator()
        :game(prepareGame())
    {}

    std::unique_ptr<core::ITask> MastermindTaskGenerator::generateTask()
    {
        return std::unique_ptr<MastermindTask>(
            new MastermindTask(game.startRandom(), CONFIG));
    }
}
