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
        :tasks(tasks), indices(), excludedSize(tasks.size()/2), excludedIndices()
    {
        for(std::size_t i = 0; i < tasks.size(); ++i)
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
