#include "task/DictTaskGenerator.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <memory>
#include <iterator>

#include "task/DictTask.h"

namespace task
{
    DictTaskGenerator::TaskCollection prepareTasks(const DictTaskGenerator::TaskCollection &tasks)
    {
        DictTaskGenerator::TaskCollection result;
        std::copy_if(tasks.begin(), tasks.end(),
            std::back_inserter(result),
            [](const DictTaskGenerator::TaskCollection::value_type &p){return !p.second.empty();});
        if(result.empty())
            throw std::runtime_error("no suitable tasks");
        return result;
    }

    IndexGenerator::IndexSet prepareIndices(std::size_t sz)
    {
        IndexGenerator::IndexSet result;
        for(std::size_t i = 0; i < sz; ++i)
            result.insert(i);
        return result;
    }

    DictTaskGenerator::DictTaskGenerator(unsigned int seed,
        const TaskCollection &tasks, bool reversed)
        :tasks(prepareTasks(tasks)), reversed(reversed),
        indexGenerator(prepareIndices(this->tasks.size()), this->tasks.size()/2, seed)
    {}

    DictTaskGenerator::~DictTaskGenerator()
    {
    }

    std::unique_ptr<core::ITask> DictTaskGenerator::generateTask()
    {
        if(!tasks.empty())
        {
            const auto idx = indexGenerator.gen();
            try
            {
                const auto &p = tasks[idx];
                if(!reversed)
                    return std::unique_ptr<core::ITask>(
                        new DictTask(p.first, p.second));
                else
                    return std::unique_ptr<core::ITask>(
                        new DictTask(p.second, p.first));
            }
            catch(const std::exception&)
            {
            }
        }
        // TODO: exception
        return std::unique_ptr<core::ITask>();
    }
}
