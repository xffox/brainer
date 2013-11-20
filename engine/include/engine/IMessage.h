#ifndef ENGINE_IMESSAGE_H
#define ENGINE_IMESSAGE_H

#include <memory>

namespace engine
{
    template<class Handler>
    class IMessage
    {
    public:
        virtual ~IMessage(){}

        virtual std::auto_ptr<IMessage> clone() const = 0;

        virtual void handle(Handler &handler) const = 0;
    };
}

#endif
