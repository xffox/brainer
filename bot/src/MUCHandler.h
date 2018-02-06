#ifndef BOT_MUCROOMHANDLER_H
#define BOT_MUCROOMHANDLER_H

#include <string>

#include <gloox/mucroom.h>
#include <gloox/mucroomhandler.h>
#include <gloox/mucroomconfighandler.h>
#include <gloox/client.h>

#include "ConferenceMessageProcessor.h"
#include "CollectingSender.h"

namespace task
{
    class TaskProvider;
}

namespace bot
{
    // TODO: own room instance?
    class MUCHandler: public gloox::MUCRoomHandler,
        public gloox::MUCRoomConfigHandler
    {
    public:
        MUCHandler(gloox::Client &client, const std::string &roomJid,
            task::TaskProvider &taskProvider);
        ~MUCHandler();

    protected:
        virtual void handleMUCConfigList(gloox::MUCRoom*, const gloox::MUCListItemList&, gloox::MUCOperation);
        virtual void handleMUCConfigForm(gloox::MUCRoom*, const gloox::DataForm&);
        virtual void handleMUCConfigResult(gloox::MUCRoom*, bool, gloox::MUCOperation);
        virtual void handleMUCRequest(gloox::MUCRoom*, const gloox::DataForm&);

        virtual void handleMUCParticipantPresence(gloox::MUCRoom*,
            gloox::MUCRoomParticipant, const gloox::Presence&);
        virtual void handleMUCMessage(gloox::MUCRoom *room,
            const gloox::Message &msg, bool priv);
        virtual bool handleMUCRoomCreation(gloox::MUCRoom*);
        virtual void handleMUCSubject(gloox::MUCRoom*, const std::string&, const std::string&);
        virtual void handleMUCInviteDecline(gloox::MUCRoom*, const gloox::JID&, const std::string&);
        virtual void handleMUCError(gloox::MUCRoom *room, gloox::StanzaError error);
        virtual void handleMUCInfo(gloox::MUCRoom*, int, const std::string&, const gloox::DataForm*);
        virtual void handleMUCItems(gloox::MUCRoom*, const gloox::Disco::ItemList&);

    private:
        gloox::MUCRoom room;
        CollectingSender sender;
        std::unique_ptr<ConferenceMessageProcessor> messageProcessor;
    };
}

#endif
