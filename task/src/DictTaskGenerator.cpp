#include "task/DictTaskGenerator.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <memory>
#include <cstdlib>
#include <iterator>

#include "task/DictTask.h"

namespace task
{
    DictTaskGenerator::DictTaskGenerator(unsigned int seed,
        const TaskCollection &tasks, bool reversed)
        :tasks(), reversed(reversed), indices(),
        excludedSize(tasks.size()/2), excludedIndices()
    {
        std::copy_if(tasks.begin(), tasks.end(),
            std::back_inserter(this->tasks),
            [](const TaskCollection::value_type &p){return !p.second.empty();});
        if(this->tasks.empty())
            throw std::runtime_error("no suitable tasks");
        for(std::size_t i = 0; i < this->tasks.size(); ++i)
            indices.push_back(i);
        srand(seed);
    }

    DictTaskGenerator::~DictTaskGenerator()
    {
    }

    std::auto_ptr<core::ITask> DictTaskGenerator::generateTask()
    {
        if(!tasks.empty())
        {
            std::size_t pos = rand()%indices.size();
            const auto idx = indices[pos];
            if(excludedSize > 0)
            {
                if(excludedIndices.size() < excludedSize)
                {
                    indices.erase(indices.begin()+pos);
                }
                else
                {
                    const auto excludedIdx = excludedIndices.front();
                    excludedIndices.pop();
                    indices[pos] = excludedIdx;
                }
                excludedIndices.push(idx);
            }
            try
            {
                const auto &p = tasks[idx];
                if(!reversed)
                    return std::auto_ptr<core::ITask>(
                        new DictTask(StringCollection{p.first}, p.second));
                else
                    return std::auto_ptr<core::ITask>(
                        new DictTask(p.second, StringCollection{p.first}));
            }
            catch(const std::exception&)
            {
            }
        }
        // TODO: exception
        return std::auto_ptr<core::ITask>();
    }
}
