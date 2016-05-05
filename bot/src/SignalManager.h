#ifndef BOT_SIGNALMANAGER_H
#define BOT_SIGNALMANAGER_H

#include <unordered_map>
#include <memory>

namespace bot
{
    class SignalManager {
    public:
        using SignalType = int;

    public:
        ~SignalManager();
        SignalManager(const SignalManager&) = delete;
        SignalManager &operator=(const SignalManager&) = delete;

        static SignalManager &getInstance();

        template<typename Func>
            void setHandler(SignalType signal, Func func);
        void clearHandler(SignalType signal);

    public:
        static const SignalType SIGNAL_TERM;
        static const SignalType SIGNAL_INT;

    private:
        class IFunc
        {
        public:
            virtual ~IFunc(){}
            virtual void run() = 0;
        };

        template<class T>
        class WrappedFunc: public IFunc
        {
        public:
            WrappedFunc(T func)
                :func(func)
            {}
            virtual void run() override
            {
                func();
            }
        private:
            T func;
        };

        typedef std::unordered_map<SignalType, std::shared_ptr<IFunc>> HandlerMap;

    private:
        SignalManager();

        void setSignalHandler(SignalType signal, std::unique_ptr<IFunc> handler);
        void handle(SignalType signum);
        void unhandle(SignalType signum);
        void blockSignals();
        void unblockSignals();

        static void managerSighandler(SignalType signum);

    private:
        HandlerMap handlers;
    };

    template<class Func>
    void SignalManager::setHandler(SignalType signal, Func func)
    {
        setSignalHandler(signal, std::unique_ptr<IFunc>(
                new WrappedFunc<Func>(func)));
    }
}

#endif
