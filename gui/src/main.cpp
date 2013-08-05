#include <QApplication>

#include "MenuDialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    gui::MenuDialog dialog;
    dialog.show();
    return app.exec();
}
