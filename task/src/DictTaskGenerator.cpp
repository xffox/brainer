#include "task/DictTaskGenerator.h"

#include <cassert>
#include <exception>
#include <memory>
#include <cstdlib>
#include <iterator>

#include "task/DictTask.h"
#include "core/IIterator.h"

namespace task
{
    DictTaskGenerator::DictTaskGenerator(unsigned int seed,
        const TaskCollection &tasks)
        :tasks(tasks)
    {
        srand(seed);
    }

    DictTaskGenerator::~DictTaskGenerator()
    {
    }

    std::auto_ptr<core::ITask> DictTaskGenerator::generateTask()
    {
        if(!tasks.empty())
        {
            std::size_t pos = rand()%tasks.size();
            try
            {
                const auto &p = tasks[pos];
                return std::auto_ptr<core::ITask>(new DictTask(p.first, p.second));
            }
            catch(const std::exception&)
            {
            }
        }
        // TODO: exception
        return std::auto_ptr<core::ITask>();
    }
}
