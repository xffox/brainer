#include "GuiConductor.h"

#include "engine/Endpoint.h"
#include "engine/IMessage.h"

namespace gui
{
    GuiConductor::GuiConductor(engine::Endpoint<core::MessageHandler> &first,
        engine::Endpoint<core::MessageHandler> &second,
        QObject *parent)
        :QObject(parent)
    {
        connect(this, SIGNAL(messageSent()), this, SLOT(handleMessage()));
        begin(first, second);
    }

    GuiConductor::~GuiConductor()
    {
        end();
    }

    void GuiConductor::send(engine::Endpoint<core::MessageHandler> &dst,
        const engine::IMessage<core::MessageHandler> &message)
    {
        Conductor::send(dst, message);
        emit messageSent();
    }

    void GuiConductor::handleMessage()
    {
        // TODO: optimize for multiple messages
        Conductor::step();
    }
}
