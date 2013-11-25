#ifndef GUI_GUIENDPOINT_H
#define GUI_GUIENDPOINT_H

#include <QObject>

#include "engine/Endpoint.h"
#include "core/MessageHandler.h"
#include "MenuDialog.h"
#include "TaskDialog.h"
#include "core/TaskId.h"

namespace gui
{
    class GuiEndpoint: public QObject,
        public engine::Endpoint<core::MessageHandler>,
        public core::MessageHandler
    {
        Q_OBJECT
    public:
        GuiEndpoint();

        virtual core::MessageHandler &handler()
        {
            return *this;
        }

        virtual void handle(const core::message::TasksMessage &message);
        virtual void handle(const core::message::DescribeMessage &message);
        virtual void handle(const core::message::ResultMessage &message);
        virtual void handle(const core::message::StatsMessage &message);
        virtual void handle(const core::message::ElapsedMessage &message);

    protected:
        virtual void started();
        virtual void stopped();

    private slots:
        void onSelected(const QString &task);
        void onEntered(const QString &value);
        void onSkipped();
        void onTimed();

    private:
        void connectToSignals();

    private:
        gui::MenuDialog menuDialog;
        gui::TaskDialog taskDialog;

        core::TaskId taskId;
    };
}

#endif
