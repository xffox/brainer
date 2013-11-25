#include "task/TaskProvider.h"

#include <exception>
#include <cstdlib>

#include "core/ITaskGenerator.h"
#include "task/HexByteTaskGenerator.h"
#include "task/MultiplicationTaskGenerator.h"

namespace task
{
    TaskProvider::TaskProvider()
        :tasks()
    {
        tasks.insert("hex");
        tasks.insert("multiplication");
    }

    const TaskProvider::StringSet &TaskProvider::getTasks() const
    {
        return tasks;
    }

    std::auto_ptr<core::ITaskGenerator> TaskProvider::create(
        const std::string &name)
    {
        if(name == "hex")
            return std::auto_ptr<core::ITaskGenerator>(
                new HexByteTaskGenerator(rand()));
        if(name == "multiplication")
            return std::auto_ptr<core::ITaskGenerator>(
                new MultiplicationTaskGenerator(rand()));
        throw std::exception();
    }
}
