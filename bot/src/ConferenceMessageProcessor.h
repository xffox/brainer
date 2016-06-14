#ifndef BOT_CONFERENCEMESSAGEPROCESSOR_H
#define BOT_CONFERENCEMESSAGEPROCESSOR_H

#include "MessageProcessor.h"

namespace bot
{
    class ConferenceMessageProcessor: public MessageProcessor
    {
    public:
        ConferenceMessageProcessor(Sender &sender, task::TaskProvider &taskProvider);

    protected:
        virtual void processAnswer(const std::string &from, const std::string &answer) override;

    private:
    };
}

#endif
