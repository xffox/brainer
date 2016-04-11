#ifndef BOT_MUCROOMHANDLER_H
#define BOT_MUCROOMHANDLER_H

#include <string>

#include <gloox/mucroom.h>
#include <gloox/mucroomhandler.h>
#include <gloox/mucroomconfighandler.h>

#include "task/TaskProvider.h"
#include "core/TaskLogic.h"

namespace bot
{
    // TODO: own room instance?
    class MUCHandler: public gloox::MUCRoomHandler,
        public gloox::MUCRoomConfigHandler
    {
    public:
        MUCHandler(gloox::MUCRoom &room, task::TaskProvider &taskProvider);

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

        void sendTask(core::TaskLogic &logic);
        void sendValid(const std::string &nick, const core::String &str);
        void sendInvalid(const std::string &nick, const core::String &str);
        void sendAnswer(const core::String &str);

    private:
        bool playTasks(const std::string &name);
        void quitTasks();

        void listTasks();
        void sendNormHelp();
        void sendPlayHelp();
        
    private:
        gloox::MUCRoom &room;
        task::TaskProvider &taskProvider;

        std::unique_ptr<core::TaskLogic> taskLogic;
    };
}

#endif
