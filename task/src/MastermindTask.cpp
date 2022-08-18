#include "task/MastermindTask.h"

#include <array>
#include <optional>
#include <ostream>
#include <sstream>
#include <utility>
#include <cwchar>

#include "core/IRender.h"

namespace task
{
    namespace
    {
        std::optional<mastermind::Pattern> parsePattern(
            const core::String &str)
        {
            std::array<core::String::value_type, 2> buf{};
            mastermind::Pattern pattern;
            for(auto c : str)
            {
                buf[0] = c;
                core::String::value_type *end{};
                const auto r = wcstol(&buf.front(), &end, 10);
                if(end != &buf.back())
                {
                    return {};
                }
                pattern.push_back(r);
            }
            return pattern;
        }

        std::wostream &writePattern(std::wostream &stream,
            const mastermind::Pattern &pattern)
        {
            for(auto v : pattern)
            {
                stream<<v;
            }
            return stream;
        }
    }

    MastermindTask::MastermindTask(const mastermind::Session &session,
            const mastermind::GameConfig &config)
        :session(session), config(config), pattern{}, feedback{}
    {}

    MastermindTask::ValidationResult MastermindTask::validate(
        const core::String &result)
    {
        const auto maybePattern = parsePattern(result);
        if(maybePattern)
        {
            const auto maybeFeedback = session.step(*maybePattern);
            if(maybeFeedback)
            {
                pattern = *maybePattern;
                feedback = *maybeFeedback;
                return {
                    session.state() == mastermind::SessionState::WON,
                    describeFeedback()
                };
            }
        }
        return {false, L"invalid pattern"};
    }

    bool MastermindTask::done() const
    {
        return session.state() != mastermind::SessionState::PLAYING;
    }

    core::String MastermindTask::answer() const
    {
        std::wstringstream ss;
        writePattern(ss, session.pattern());
        return std::move(ss).str();
    }

    void MastermindTask::describe(core::IRender &render) const
    {
        std::wstringstream ss;
        ss<<"new code: "<<config.length<<" digits of "
            <<config.colors<<" values "
            <<(config.repeats?"with":"without")<<" repeats"
            <<" in "<<config.guesses<<" guesses";
        render.addText(std::move(ss).str());
    }

    void MastermindTask::hint(core::IRender&, std::size_t) const
    {}

    core::String MastermindTask::describeFeedback() const
    {
        std::wstringstream ss;
        writePattern(ss, pattern);
        ss<<" - white: "<<feedback.white<<", black: "<<feedback.black
            <<", guesses left: "<<(config.guesses - session.guessIndex());
        return std::move(ss).str();
    }
}
