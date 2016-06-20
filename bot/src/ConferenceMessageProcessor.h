#ifndef BOT_CONFERENCEMESSAGEPROCESSOR_H
#define BOT_CONFERENCEMESSAGEPROCESSOR_H

#include <unordered_map>
#include <cstddef>

#include "MessageProcessor.h"

namespace bot
{
    class ConferenceMessageProcessor: public MessageProcessor
    {
    public:
        ConferenceMessageProcessor(Sender &sender, task::TaskProvider &taskProvider);

    protected:
        virtual base::Nullable<Validity> processAnswer(const std::string &from, const std::string &answer) override;
        virtual void processQuitCmd(const std::string &from, const StringList &args) override;

        virtual void sendInvalid(const std::string &from, const core::String &descr,
            const core::String &str) override;
        virtual void sendValid(const std::string &from, const core::String &descr,
            const core::String &str, const core::TaskLogic::StatsCol &stats) override;
        virtual void sendStats(const core::TaskLogic::StatsCol &stats) override;

    private:
        struct Score
        {
            Score():right(0), wrong(0){}
            Score(std::size_t right, std::size_t wrong)
                :right(right), wrong(wrong){}
            std::size_t right;
            std::size_t wrong;
        };
        using ScoreCountMap = std::unordered_map<std::string, Score>;

    private:
        ScoreCountMap scores;
    };
}

#endif
