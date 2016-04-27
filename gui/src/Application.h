#ifndef GUI_APPLICATION_H
#define GUI_APPLICATION_H

#include <QApplication>

#include "MenuDialog.h"

namespace gui
{
    class Application: public QApplication
    {
        Q_OBJECT
    public:
        Application(int &argc, char **argv);
        virtual ~Application();

    private:
        gui::MenuDialog menuDialog;
    };
}

#endif
