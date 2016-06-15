#include "MUCHandler.h"

#include <cassert>
#include <sstream>

#include <gloox/message.h>

#include "xlog/xlog.h"

#include "core/ITaskGenerator.h"

namespace bot
{
    MUCHandler::MUCHandler(gloox::MUCRoom &room,
        task::TaskProvider &taskProvider)
        :room(room), sender(), messageProcessor(new ConferenceMessageProcessor(sender, taskProvider))
    {}

    void MUCHandler::handleMUCConfigList(gloox::MUCRoom*, const gloox::MUCListItemList&, gloox::MUCOperation)
    {
    }

    void MUCHandler::handleMUCConfigForm(gloox::MUCRoom*, const gloox::DataForm&)
    {
    }

    void MUCHandler::handleMUCConfigResult(gloox::MUCRoom*, bool, gloox::MUCOperation)
    {
    }

    void MUCHandler::handleMUCRequest(gloox::MUCRoom*, const gloox::DataForm&)
    {
    }

    void MUCHandler::handleMUCParticipantPresence(gloox::MUCRoom*,
        gloox::MUCRoomParticipant, const gloox::Presence&)
    {
    }
    bool MUCHandler::handleMUCRoomCreation(gloox::MUCRoom*)
    {
        return true;
    }

    void MUCHandler::handleMUCSubject(gloox::MUCRoom*, const std::string&, const std::string&)
    {
    }

    void MUCHandler::handleMUCInviteDecline(gloox::MUCRoom*, const gloox::JID&, const std::string&)
    {
    }

    void MUCHandler::handleMUCError(gloox::MUCRoom *room,
        gloox::StanzaError error)
    {
        xlog::log().error("MUCHandler", "muc error: %d", error);
    }

    void MUCHandler::handleMUCInfo(gloox::MUCRoom*, int, const std::string&, const gloox::DataForm*)
    {
    }

    void MUCHandler::handleMUCItems(gloox::MUCRoom*, const gloox::Disco::ItemList&)
    {
    }

    void MUCHandler::handleMUCMessage(gloox::MUCRoom*,
        const gloox::Message &msg, bool priv)
    {
        assert(messageProcessor.get());
        if(priv || msg.from().resource() == room.nick())
            return;
        if(msg.subtype() == gloox::Message::Groupchat)
        {
            messageProcessor->receive(msg.from().resource(), msg.body());
            const auto &msgs = sender.getMessages();
            std::stringstream stream;
            for(CollectingSender::StringCol::size_type i = 0; i < msgs.size(); ++i)
            {
                stream<<msgs[i];
                if(i+1 < msgs.size())
                    stream<<std::endl;
            }
            room.send(stream.str());
            sender.reset();
        }
    }
}
