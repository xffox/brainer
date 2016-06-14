#include "ConferenceMessageProcessor.h"

#include "strutil.h"

namespace bot
{
    ConferenceMessageProcessor::ConferenceMessageProcessor(Sender &sender, task::TaskProvider &taskProvider)
        :MessageProcessor(sender, taskProvider)
    {
    }

    void ConferenceMessageProcessor::processAnswer(const std::string &from, const std::string &answer)
    {
        if(getTaskLogic())
        {
            const auto ans = strutil::toCoreString(answer);
            const auto descr = description(*getTaskLogic());
            if(answerTask(ans))
            {
                sendValid(from, descr, ans, *getTaskLogic());
                sendTask(*getTaskLogic());
            }
            else
            {
                sendInvalid(from, descr, ans, *getTaskLogic());
            }
        }
        else
        {
            send("no game is played now");
            sendNormHelp();
        }
    }
}
