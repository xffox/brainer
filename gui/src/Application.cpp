#include "Application.h"

#include <memory>

#include "task/TaskProvider.h"

namespace gui
{
    Application::Application(int &argc, char **argv)
        :QApplication(argc, argv), coreEndpoint(
            std::auto_ptr<core::ITaskProvider>(new task::TaskProvider())),
        guiEndpoint(), conductor(coreEndpoint, guiEndpoint)
    {
    }

    Application::~Application()
    {
    }
}
