#include "MenuDialog.h"

#include <ctime>
#include <memory>

#include "task/MultiplicationTaskGenerator.h"
#include "task/HexByteTaskGenerator.h"
#include "TaskDialog.h"

namespace gui
{
    MenuDialog::MenuDialog(QWidget *parent)
        :QDialog(parent)
    {
        ui.setupUi(this);

        connectToSignals();
    }

    void MenuDialog::onHexByte()
    {
        TaskDialog *dialog = new TaskDialog(std::auto_ptr<core::ITaskGenerator>(
                new task::HexByteTaskGenerator(time(0))));
        dialog->show();
    }

    void MenuDialog::onMultiplication()
    {
        TaskDialog *dialog = new TaskDialog(std::auto_ptr<core::ITaskGenerator>(
                new task::MultiplicationTaskGenerator(time(0))));
        dialog->show();
    }

    void MenuDialog::connectToSignals()
    {
        connect(ui.hexByteButton, SIGNAL(clicked()), this, SLOT(onHexByte()));
        connect(ui.multiplicationButton, SIGNAL(clicked()), this, SLOT(onMultiplication()));
    }
}
