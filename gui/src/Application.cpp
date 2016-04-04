#include "Application.h"

#include <memory>

#include "core/ITaskProvider.h"
#include "task/TaskProvider.h"

namespace gui
{
    Application::Application(int &argc, char **argv)
        :QApplication(argc, argv),
        menuDialog(std::unique_ptr<core::ITaskProvider>(
                new task::TaskProvider()))
    {
        menuDialog.show();
    }

    Application::~Application()
    {
    }
}
