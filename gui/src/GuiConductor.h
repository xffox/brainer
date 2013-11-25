#ifndef GUI_GUICONDUCTOR_H
#define GUI_GUICONDUCTOR_H

#include <QObject>

#include "engine/Conductor.h"
#include "core/MessageHandler.h"

namespace engine
{
    template<class Handler>
    class Endpoint;
    template<class Handler>
    class IMessage;
}

namespace gui
{
    class GuiConductor: public QObject,
        public engine::Conductor<core::MessageHandler>
    {
        Q_OBJECT
    public:
        GuiConductor(engine::Endpoint<core::MessageHandler> &first,
            engine::Endpoint<core::MessageHandler> &second,
            QObject *parent = 0);
        virtual ~GuiConductor();

    signals:
        void messageSent();

    protected:
        virtual void send(engine::Endpoint<core::MessageHandler> &dst,
            const engine::IMessage<core::MessageHandler> &message);
        // TODO: probably handle stop() too

    private slots:
        void handleMessage();
    };
}

#endif
