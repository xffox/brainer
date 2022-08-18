#ifndef BOT_ROSTERMANAGER_HPP
#define BOT_ROSTERMANAGER_HPP

#include <gloox/jid.h>
#include <gloox/rostermanager.h>
#include <gloox/rosterlistener.h>

namespace bot
{
    class RosterManager: public gloox::RosterListener
    {
    public:
        explicit RosterManager(gloox::RosterManager &manager);
        ~RosterManager();

        bool isKnown(const gloox::JID &jid) const;

    protected:
        void handleItemAdded(const gloox::JID&) override
        {}
        void handleItemSubscribed(const gloox::JID &jid) override;
        void handleItemRemoved(const gloox::JID&) override
        {}
        void handleItemUpdated(const gloox::JID&) override
        {}
        void handleItemUnsubscribed(const gloox::JID &jid) override;
        void handleRoster(const gloox::Roster&) override
        {}
        void handleRosterPresence(const gloox::RosterItem&,
            const std::string&, gloox::Presence::PresenceType,
            const std::string&) override
        {}
        void handleSelfPresence(const gloox::RosterItem&,
            const std::string&, gloox::Presence::PresenceType,
            const std::string&) override
        {}
        bool handleSubscriptionRequest(
            const gloox::JID &jid, const std::string &msg) override;
        bool handleUnsubscriptionRequest(const gloox::JID&,
            const std::string&) override;
        void handleNonrosterPresence(const gloox::Presence&) override
        {}
        void handleRosterError(const gloox::IQ&) override
        {}

    private:
        gloox::RosterManager &manager;
    };
}

#endif
