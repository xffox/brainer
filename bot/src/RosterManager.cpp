#include "RosterManager.hpp"

#include <xlog/xlog.hpp>

namespace bot
{
    RosterManager::RosterManager(gloox::RosterManager &manager)
        :manager(manager)
    {
        manager.registerRosterListener(this, true);
    }

    RosterManager::~RosterManager()
    {
        manager.removeRosterListener();
    }

    bool RosterManager::isKnown(const gloox::JID &jid) const
    {
        const auto *const item = manager.getRosterItem(jid);
        if(item)
        {
            const auto subscriptionType = item->subscription();
            return subscriptionType == gloox::S10nTo ||
                subscriptionType == gloox::S10nToIn ||
                subscriptionType == gloox::S10nBoth;
        }
        return false;
    }

    bool RosterManager::handleSubscriptionRequest(
        const gloox::JID &jid, const std::string&)
    {
        manager.subscribe(jid);
        xlog::log().info("RosterManager",
            "subscribing: %s", jid.bare().c_str());
        return true;
    }

    bool RosterManager::handleUnsubscriptionRequest(const gloox::JID &jid,
        const std::string&)
    {
        manager.unsubscribe(jid);
        xlog::log().info("RosterManager",
            "unsubscribing: %s", jid.bare().c_str());
        return true;
    }

    void RosterManager::handleItemSubscribed(const gloox::JID &jid)
    {
        xlog::log().info("RosterManager",
            "subscribed: %s", jid.bare().c_str());
    }

    void RosterManager::handleItemUnsubscribed(const gloox::JID &jid)
    {
        xlog::log().info("RosterManager",
            "unsubscribed: %s", jid.bare().c_str());
    }
}
