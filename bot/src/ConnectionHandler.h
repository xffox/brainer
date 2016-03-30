#ifndef BOT_CONNECTIONHANDLER_H
#define BOT_CONNECTIONHANDLER_H

#include <gloox/connectionlistener.h>

namespace bot
{
    class ConnectionHandler: public gloox::ConnectionListener
    {
    public:

    protected:
        virtual void onConnect();
        virtual void onDisconnect(gloox::ConnectionError e);
        virtual bool onTLSConnect(const gloox::CertInfo& info);
        
    private:
        
    };
}

#endif
