#include "Application.h"

#include <memory>

#include <QDir>

#include <base/randomizer.hpp>
#include "core/ITaskProvider.h"
#include "task/TaskProvider.h"

namespace gui
{
    Application::Application(int &argc, char **argv)
        :QApplication(argc, argv),
        menuDialog(std::unique_ptr<core::ITaskProvider>(
                new task::TaskProvider(base::Randomizer(),
                    (QDir::homePath() + QDir::separator() +
                     QString(".brainer_tasks.csv")).toStdString())))
    {
        menuDialog.show();
    }

    Application::~Application() = default;
}
