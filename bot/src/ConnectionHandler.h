#ifndef BOT_CONNECTIONHANDLER_H
#define BOT_CONNECTIONHANDLER_H

#include <string>

#include <gloox/connectionlistener.h>
#include <gloox/client.h>

namespace bot
{
    class ConnectionHandler: public gloox::ConnectionListener
    {
    public:
        ConnectionHandler(gloox::Client &client);

    protected:
        virtual void onConnect();
        virtual void onDisconnect(gloox::ConnectionError e);
        virtual bool onTLSConnect(const gloox::CertInfo& info);
        
    private:
        gloox::Client &client;
    };
}

#endif
