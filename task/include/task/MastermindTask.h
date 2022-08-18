#ifndef TASK_MASTERMINDTASK_H
#define TASK_MASTERMINDTASK_H

#include "core/ITask.h"
#include "core/String.h"
#include "mastermind/session.hpp"
#include "mastermind/config.hpp"
#include "mastermind/type.hpp"

namespace task
{
    class MastermindTask: public core::ITask
    {
        friend class MastermindTaskGenerator;
    public:
        ValidationResult validate(const core::String &result) override;
        bool done() const override;
        core::String answer() const override;
        void describe(core::IRender &render) const override;
        void hint(core::IRender &render, std::size_t level) const override;

    private:
        explicit MastermindTask(const mastermind::Session &session,
            const mastermind::GameConfig &config);

        core::String describeFeedback() const;

    private:
        mastermind::Session session;
        mastermind::GameConfig config;
        mastermind::Pattern pattern;
        mastermind::Feedback feedback;
    };
}

#endif
