#include "Application.h"

#include <memory>

#include <QDir>

#include "core/ITaskProvider.h"
#include "task/TaskProvider.h"

namespace gui
{
    Application::Application(int &argc, char **argv)
        :QApplication(argc, argv),
        menuDialog(std::unique_ptr<core::ITaskProvider>(
                new task::TaskProvider((QDir::homePath() + QDir::separator() + QString(".brainer_tasks.csv")).toStdString())))
    {
        menuDialog.show();
    }

    Application::~Application()
    {
    }
}
