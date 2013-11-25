#ifndef GUI_APPLICATION_H
#define GUI_APPLICATION_H

#include <QApplication>

#include "engine/Conductor.h"
#include "core/CoreEndpoint.h"
#include "core/MessageHandler.h"
#include "GuiEndpoint.h"
#include "GuiConductor.h"

namespace gui
{
    class Application: public QApplication
    {
        Q_OBJECT
    public:
        Application(int &argc, char **argv);
        virtual ~Application();

    private:
        void setup();

    private:
        core::CoreEndpoint coreEndpoint;
        GuiEndpoint guiEndpoint;
        GuiConductor conductor;
    };
}

#endif
