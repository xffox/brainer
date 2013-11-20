#ifndef ENGINE_ENDPOINT_H
#define ENGINE_ENDPOINT_H

#include <exception>

namespace engine
{
    template<class Handler>
    class IMessage;

    template<class Handler>
    class Conductor;

    template<class Handler>
    class Endpoint
    {
        template<class H>
        friend class Conductor;
    public:
        Endpoint()
        :conductor(), dst()
        {}

        virtual ~Endpoint(){}

        // TODO: protected
        virtual Handler &handler() = 0;

    protected:
        virtual void started() = 0;
        virtual void stopped() = 0;

        void send(const IMessage<Handler> &message);
        void stop();

    private:
        void setDestination(Conductor<Handler> *conductor,
            Endpoint<Handler> *dst);

    private:
        Conductor<Handler> *conductor;
        Endpoint<Handler> *dst;
    };

    template<class Handler>
    void Endpoint<Handler>::send(const IMessage<Handler> &message)
    {
        if(conductor && dst)
            conductor->send(*dst, message);
        else
            // TODO: proper exception
            throw std::exception();
    }

    template<class Handler>
    void Endpoint<Handler>::setDestination(Conductor<Handler> *conductor,
        Endpoint<Handler> *dst)
    {
        this->conductor = conductor;
        this->dst = dst;
    }

    template<class Handler>
    void Endpoint<Handler>::stop()
    {
        if(conductor)
            conductor->stop();
        else
            throw std::exception();
    }
}

#endif
