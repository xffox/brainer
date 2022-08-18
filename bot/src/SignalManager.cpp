#include "SignalManager.h"

#include <utility>
#include <cerrno>
#include <cstring>
#include <cassert>
#include <exception>
#include <stdexcept>

#include <signal.h>

namespace bot
{
    SignalManager::SignalManager()
        :handlers(){}

    SignalManager::~SignalManager()
    {
        try
        {
            blockSignals();
            try
            {
                for(const auto &p : handlers)
                {
                    try
                    {
                        unhandle(p.first);
                    }
                    catch(const std::exception&)
                    {}
                }
            }
            catch(const std::exception&)
            {}
            unblockSignals();
        }
        catch(const std::exception&)
        {}
    }

    SignalManager &SignalManager::getInstance()
    {
        static SignalManager instance;
        return instance;
    }

    void SignalManager::setSignalHandler(SignalType signal,
        std::unique_ptr<IFunc> handler)
    {
        blockSignals();
        try
        {
            auto iter = handlers.find(signal);
            if(iter == handlers.end())
            {
                iter = handlers.insert(std::make_pair(signal,
                        std::shared_ptr<IFunc>(handler.release()))).first;
                try
                {
                    handle(signal);
                }
                catch(const std::exception&)
                {
                    handlers.erase(iter);
                    throw;
                }
            }
            else
            {
                iter->second = std::shared_ptr<IFunc>(handler.release());
            }
        }
        catch(const std::exception&)
        {
            try
            {
                unblockSignals();
            }
            catch(const std::exception&)
            {
                throw std::runtime_error("signals in invalid state");
            }
            throw;
        }
        try
        {
            unblockSignals();
        }
        catch(const std::exception&)
        {
            throw std::runtime_error("signals in invalid state");
        }
    }

    void SignalManager::clearHandler(SignalType signal)
    {
        blockSignals();
        try
        {
            auto iter = handlers.find(signal);
            if(iter == handlers.end())
                throw std::invalid_argument("signal not handled");
            unhandle(signal);
            handlers.erase(iter);
        }
        catch(const std::exception&)
        {
            try
            {
                unblockSignals();
            }
            catch(const std::exception&)
            {
                throw std::runtime_error("signals in invalid state");
            }
            throw;
        }
        try
        {
            unblockSignals();
        }
        catch(const std::exception&)
        {
            throw std::runtime_error("signals in invalid state");
        }
    }

    void SignalManager::handle(SignalType signum)
    {
        if(signal(signum, managerSighandler) == SIG_ERR)
            throw std::runtime_error(strerror(errno));
    }

    void SignalManager::unhandle(SignalType signum)
    {
        if(signal(signum, SIG_DFL) == SIG_ERR)
            throw std::runtime_error(strerror(errno));
    }

    void SignalManager::blockSignals()
    {
        sigset_t signals;
        sigemptyset(&signals);
        for(const auto &p : handlers)
            sigaddset(&signals, p.first);
        if(sigprocmask(SIG_SETMASK, &signals, nullptr) != 0)
            throw std::runtime_error(strerror(errno));
    }

    void SignalManager::unblockSignals()
    {
        sigset_t signals;
        sigemptyset(&signals);
        if(sigprocmask(SIG_SETMASK, &signals, nullptr) != 0)
            throw std::runtime_error(strerror(errno));
    }

    void SignalManager::managerSighandler(SignalType signum)
    {
        SignalManager &manager = getInstance();
        HandlerMap::iterator iter = manager.handlers.find(signum);
        assert(iter != manager.handlers.end());
        assert(iter->second.get());
        iter->second->run();
    }

    const SignalManager::SignalType SignalManager::SIGNAL_TERM = SIGTERM;
    const SignalManager::SignalType SignalManager::SIGNAL_INT = SIGINT;
}
