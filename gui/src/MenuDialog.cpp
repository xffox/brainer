#include "MenuDialog.h"

#include <algorithm>
#include <cassert>
#include <ctime>
#include <exception>
#include <iterator>
#include <memory>
#include <utility>
#include <QtGui/QFileDialog>

#include "core/FileDictProvider.h"
#include "TaskDialog.h"
#include "task/DictTaskGenerator.h"
#include "task/HexByteTaskGenerator.h"
#include "task/MultiplicationTaskGenerator.h"

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
        TaskDialog *dialog =
            new TaskDialog(std::auto_ptr<core::ITaskGenerator>(
                    new task::HexByteTaskGenerator(time(0))));
        dialog->show();
    }

    void MenuDialog::onMultiplication()
    {
        TaskDialog *dialog =
            new TaskDialog(std::auto_ptr<core::ITaskGenerator>(
                    new task::MultiplicationTaskGenerator(time(0))));
        dialog->show();
    }

    void MenuDialog::onDict()
    {
        const QString path = QFileDialog::getOpenFileName(this);
        if(!path.isEmpty())
        {
            TaskDialog *dialog =
                new TaskDialog(std::auto_ptr<core::ITaskGenerator>(
                        new task::DictTaskGenerator(time(0),
                            std::auto_ptr<core::IDictProvider>(
                                new core::FileDictProvider(qPrintable(path))))
                        ));
            dialog->show();
        }
    }

    void MenuDialog::connectToSignals()
    {
        connect(ui.hexByteButton, SIGNAL(clicked()), this, SLOT(onHexByte()));
        connect(ui.multiplicationButton, SIGNAL(clicked()), this,
            SLOT(onMultiplication()));
        connect(ui.dictButton, SIGNAL(clicked()), this, SLOT(onDict()));
    }
}
