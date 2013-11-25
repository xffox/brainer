#ifndef ENGINE_LOOPCONDUCTOR_H
#define ENGINE_LOOPCONDUCTOR_H

#include <condition_variable>
#include <mutex>

#include "engine/Conductor.h"

namespace engine
{
    template<class Handler>
    class LoopConductor: public Conductor<Handler>
    {
    public:
        LoopConductor();

        void run(Endpoint<Handler> &first, Endpoint<Handler> &second);
        void kill();

    protected:
        virtual void send(Endpoint<Handler> &dst,
            const IMessage<Handler> &message);
        virtual void stop();

    private:
        std::mutex mutex;
        std::condition_variable condition;
    };

    template<class Handler>
    LoopConductor<Handler>::LoopConductor()
    :mutex(), condition()
    {}

    template<class Handler>
    void LoopConductor<Handler>::run(Endpoint<Handler> &first,
        Endpoint<Handler> &second)
    {
        Conductor<Handler>::begin(first, second);
        {
            std::unique_lock<std::mutex> lock(mutex);
            while(Conductor<Handler>::isRunning())
            {
                if(!Conductor<Handler>::step())
                {
                    condition.wait(lock);
                }
            }
        }
        Conductor<Handler>::end();
    }

    template<class Handler>
    void LoopConductor<Handler>::kill()
    {
        stop();
    }

    template<class Handler>
    void LoopConductor<Handler>::stop()
    {
        std::lock_guard<std::mutex> lock(mutex);
        Conductor<Handler>::stop();
        condition.notify_one();
    }

    template<class Handler>
    void LoopConductor<Handler>::send(Endpoint<Handler> &dst,
        const IMessage<Handler> &message)
    {
        std::lock_guard<std::mutex> lock(mutex);
        Conductor<Handler>::send(dst, message);
        condition.notify_one();
    }
}

#endif
