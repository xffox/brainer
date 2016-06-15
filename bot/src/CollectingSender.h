#ifndef BOT_COLLECTINGSENDER_H
#define BOT_COLLECTINGSENDER_H

#include <vector>
#include <string>

#include "MessageProcessor.h"

namespace bot
{
    class CollectingSender: public MessageProcessor::Sender
    {
    public:
        using StringCol = std::vector<std::string>;

    public:
        virtual void send(const std::string &msg) override;

        // TODO: destroy on retrieving the messages
        const StringCol &getMessages() const
        {
            return messages;
        }

        void reset()
        {
            messages.clear();
        }

    private:
        StringCol messages;
    };
}

#endif
