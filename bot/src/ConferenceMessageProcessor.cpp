#include "ConferenceMessageProcessor.h"

#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <set>
#include <utility>

#include "strutil.h"

namespace bot
{
    ConferenceMessageProcessor::ConferenceMessageProcessor(Sender &sender, task::TaskProvider &taskProvider)
        :MessageProcessor(sender, taskProvider), scores()
    {
    }

    std::optional<ConferenceMessageProcessor::Validity> ConferenceMessageProcessor::processAnswer(
        const std::string &from, const std::string &answer)
    {
        const auto res = MessageProcessor::processAnswer(from, answer);
        if(res)
        {
            if(*res == VALID)
            {
                scores[from].right += 1;
            }
            else
            {
                scores[from].wrong += 1;
            }
        }
        return res;
    }

    void ConferenceMessageProcessor::processQuitCmd(const std::string &from, const StringList &args)
    {
        MessageProcessor::processQuitCmd(from, args);
        scores.clear();
    }

    void ConferenceMessageProcessor::sendInvalid(const std::string &from, const std::optional<core::String> &descr,
        const core::String &answer)
    {
        std::wstringstream stream;
        stream<<strutil::toCoreString(from)<<", WRONG: "<<answer;
        if(descr)
        {
            stream<<" - "<<*descr;
        }
        send(strutil::fromCoreString(std::move(stream).str()));
    }

    void ConferenceMessageProcessor::sendValid(const std::string &from, const std::optional<core::String>&,
        const core::String &answer, const core::TaskLogic::StatsCol &stats)
    {
        std::wstringstream stream;
        if(stats.empty())
        {
            throw std::runtime_error("stats list is empty on valid result");
        }
        const auto elapsed = stats.back().time;
        stream<<strutil::toCoreString(from)<<L", RIGHT: "<<answer
            <<" ("<<std::setprecision(2)<<std::fixed
            <<static_cast<double>(elapsed.count())/1000000.0<<"s)";
        send(strutil::fromCoreString(stream.str()));
    }

    void ConferenceMessageProcessor::sendStats(const core::TaskLogic::StatsCol &stats)
    {
        struct SortScore
        {
            std::string name;
            Score score;
        };
        struct ScoreLess
        {
            bool operator()(const SortScore &left, const SortScore &right) const
            {
                return left.score.right > right.score.right ||
                    (left.score.right == right.score.right &&
                     (left.score.wrong < right.score.wrong ||
                      (left.score.wrong == right.score.wrong &&
                       left.name < right.name)));
            }
        };
        using SortedScoreSet = std::set<SortScore, ScoreLess>;
        if(!scores.empty())
        {
            SortedScoreSet sortedScores;
            std::transform(scores.begin(), scores.end(),
                std::inserter(sortedScores, sortedScores.begin()),
                [](const ScoreCountMap::value_type &p){return SortScore{p.first, p.second};});
            std::stringstream stream;
            stream<<"scores:"<<std::endl;
            for(auto iter = sortedScores.begin(); iter != sortedScores.end(); ++iter)
            {
                const auto &p = *iter;
                stream<<p.name<<": right:"<<p.score.right<<", wrong:"<<p.score.wrong;
                auto nextIter = iter;
                ++nextIter;
                if(nextIter != sortedScores.end())
                {
                    stream<<std::endl;
                }
            }
            send(stream.str());
        }
        MessageProcessor::sendStats(stats);
    }
}
