#include <iostream>
#include <memory>

#include "engine/LoopConductor.h"
#include "engine/Endpoint.h"
#include "engine/IMessage.h"

namespace
{
    unsigned int MESSAGES = 1000;
}

namespace
{
    class DataMessage;
    class AckMessage;

    class TestHandler
    {
    public:
        virtual ~TestHandler(){}

        virtual void handle(const DataMessage&)
        {
        }

        virtual void handle(const AckMessage&)
        {
        }
    };

    class DataMessage: public engine::IMessage<TestHandler>
    {
    public:
        virtual std::auto_ptr<engine::IMessage<TestHandler> > clone() const
        {
            return std::auto_ptr<engine::IMessage<TestHandler> >(
                new DataMessage(*this));
        }
        virtual void handle(TestHandler &handler) const
        {
            handler.handle(*this);
        }
    };

    class AckMessage: public engine::IMessage<TestHandler>
    {
        virtual std::auto_ptr<engine::IMessage<TestHandler> > clone() const
        {
            return std::auto_ptr<engine::IMessage<TestHandler> >(
                new AckMessage(*this));
        }
        virtual void handle(TestHandler &handler) const
        {
            handler.handle(*this);
        }
    };

    class TestSender: public engine::Endpoint<TestHandler>,
        public TestHandler
    {
    public:
        TestSender()
        :msgs(0)
        {}

        virtual TestHandler &handler()
        {
            return *this;
        }

        void handle(const AckMessage &msg)
        {
            if(msgs < MESSAGES)
                sendData();
            else
                stop();
        }

        unsigned int getMessageCount() const
        {
            return msgs;
        }

    protected:
        virtual void started()
        {
            sendData();
        }

        virtual void stopped()
        {
        }

    private:
        void sendData()
        {
            send(DataMessage());
            ++msgs;
        }

    private:
        unsigned int msgs;
    };

    class TestReceiver: public engine::Endpoint<TestHandler>,
        public TestHandler
    {
    public:
        TestReceiver()
        :msgs(0)
        {}

        virtual TestHandler &handler()
        {
            return *this;
        }

        void handle(const DataMessage &msg)
        {
            ++msgs;
            send(AckMessage());
        }

        unsigned int getMessageCount() const
        {
            return msgs;
        }

    protected:
        virtual void started()
        {
        }

        virtual void stopped()
        {
        }

    private:
        unsigned int msgs;
    };
}

int main()
{
    TestSender sender;
    TestReceiver receiver;
    engine::LoopConductor<TestHandler> conductor;
    conductor.run(sender, receiver);
    if(sender.getMessageCount() != receiver.getMessageCount())
    {
        std::cerr<<"message count mismatch: send="<<sender.getMessageCount()
            <<" recv="<<receiver.getMessageCount()<<std::endl;
        return 1;
    }
    else
    {
        std::cout<<"message count match: "<<sender.getMessageCount()
            <<std::endl;
    }
    return 0;
}
