#ifndef ENGINE_CONDUCTOR_H
#define ENGINE_CONDUCTOR_H

#include <cassert>
#include <exception>
#include <queue>

#include "engine/IMessage.h"
#include "engine/Endpoint.h"

namespace engine
{
    template<class Handler>
    class Endpoint;

    template<class Handler>
    class Conductor
    {
        template<class H>
        friend class Endpoint;
    public:
        Conductor();
        virtual ~Conductor()
        {}

        virtual void begin(Endpoint<Handler> &first,
            Endpoint<Handler> &second);
        virtual bool step();
        virtual void end();

    protected:
        struct Message
        {
            Message(IMessage<Handler> *message, Endpoint<Handler> *dst)
            :message(message), dst(dst)
            {}

            IMessage<Handler> *message;
            Endpoint<Handler> *dst;
        };
        typedef std::queue<Message> MessageQueue;

    protected:
        // not thread-safe
        virtual void send(Endpoint<Handler> &dst,
            const IMessage<Handler> &message);
        virtual void stop();

        bool isRunning() const
        {
            return running;
        }

    protected:
        MessageQueue queue;

        volatile bool running;

        Endpoint<Handler> *first;
        Endpoint<Handler> *second;
    };

    template<class Handler>
    Conductor<Handler>::Conductor()
    :queue(), running(true), first(0), second(0)
    {}

    template<class Handler>
    void Conductor<Handler>::begin(Endpoint<Handler> &first,
        Endpoint<Handler> &second)
    {
        if(this->first)
            // TODO: proper exception
            throw std::exception();
        this->first = &first;
        this->second = &second;
        first.setDestination(this, &second);
        second.setDestination(this, &first);
        // TODO: handle exceptions
        first.started();
        second.started();
    }

    template<class Handler>
    bool Conductor<Handler>::step()
    {
        if(running && !queue.empty())
        {
            IMessage<Handler> *msg = queue.front().message;
            Endpoint<Handler> *dst = queue.front().dst;
            queue.pop();
            std::auto_ptr<IMessage<Handler> > m(msg);
            assert(m.get());
            assert(dst);
            try
            {
                m->handle(dst->handler());
            }
            catch(...)
            {
            }
            return true;
        }
        return false;
    }

    template<class Handler>
    void Conductor<Handler>::end()
    {
        if(!first)
            // TODO: proper exception
            throw std::exception();
        assert(second);
        second->stopped();
        first->stopped();
        second->setDestination(0, 0);
        first->setDestination(0, 0);
        first = 0;
        second = 0;
    }

    template<class Handler>
    void Conductor<Handler>::send(Endpoint<Handler> &dst,
        const IMessage<Handler> &message)
    {
        std::auto_ptr<IMessage<Handler> > m(message.clone());
        queue.push(Message(m.get(), &dst));
        m.release();
    }

    template<class Handler>
    void Conductor<Handler>::stop()
    {
        running = false;
    }
}

#endif
