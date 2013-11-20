#ifndef ENGINE_CONDUCTOR_H
#define ENGINE_CONDUCTOR_H

#include <cassert>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace engine
{
    template<class Handler>
    class Endpoint;

    template<class Handler>
    class IMessage;

    template<class Handler>
    class Conductor
    {
        template<class H>
        friend class Endpoint;
    public:
        Conductor();

        void run(Endpoint<Handler> &first, Endpoint<Handler> &second);
        void stop();

    private:
        struct Message
        {
            Message(IMessage<Handler> *message, Endpoint<Handler> *dst)
            :message(message), dst(dst)
            {}

            IMessage<Handler> *message;
            Endpoint<Handler> *dst;
        };
        typedef std::queue<Message> MessageQueue;

    private:
        void send(Endpoint<Handler> &dst, const IMessage<Handler> &message);

        void wake();

    private:
        volatile bool running;

        MessageQueue queue;

        std::mutex mutex;
        std::condition_variable condition;
    };

    template<class Handler>
    Conductor<Handler>::Conductor()
    :running(true), mutex(), condition()
    {}

    template<class Handler>
    void Conductor<Handler>::run(Endpoint<Handler> &first,
        Endpoint<Handler> &second)
    {
        first.setDestination(this, &second);
        second.setDestination(this, &first);
        // TODO: handle exceptions
        first.started();
        second.started();
        {
            std::unique_lock<std::mutex> lock(mutex);
            while(running)
            {
                if(!queue.empty())
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
                }
                condition.wait(lock);
            }
        }
        second.stopped();
        first.stopped();
        second.setDestination(0, 0);
        first.setDestination(0, 0);
    }

    template<class Handler>
    void Conductor<Handler>::stop()
    {
        // TODO: make this safe
        running = false;
        wake();
    }

    template<class Handler>
    void Conductor<Handler>::send(Endpoint<Handler> &dst,
        const IMessage<Handler> &message)
    {
        std::auto_ptr<IMessage<Handler> > m(message.clone());
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(Message(m.get(), &dst));
        m.release();
        condition.notify_one();
    }

    template<class Handler>
    void Conductor<Handler>::wake()
    {
        std::lock_guard<std::mutex> lock(mutex);
        condition.notify_one();
    }
}

#endif
