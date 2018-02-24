#include "task/DictTask.h"

#include <sstream>
#include <iterator>
#include <stdexcept>
#include <random>
#include <vector>

#include "base/strutil.h"
#include "core/IRender.h"

#include "util.h"

namespace task
{
    DictTask::DictTask(const StringCollection &keys,
        const StringCollection &values, int seed)
        :keys(keys), values(values), answers(), seed(seed)
    {
        if(values.empty() || keys.empty())
            throw std::runtime_error("task values or keys are empty");
        util::toLowerCollection(keys.begin(), keys.end(),
            std::inserter(answers, answers.begin()));
    }

    bool DictTask::validate(const core::String &result) const
    {
        return answers.find(util::tolower(result)) != answers.end();
    }

    core::String DictTask::answer() const
    {
        return base::strutil::join(keys.begin(), keys.end(),
            core::String(L","));
    }

    void DictTask::describe(core::IRender &render) const
    {
        render.addText(base::strutil::join(values.begin(), values.end(),
                core::String(L",")));
    }

    void DictTask::hint(core::IRender &render, std::size_t) const
    {
        using IdxCollection = std::vector<std::size_t>;
        std::seed_seq seedSeq({seed});
        std::minstd_rand random(seedSeq);
        if(!keys.empty())
        {
            const auto &key = keys.front();
            const auto level = key.size()/2;
            IdxCollection indices;
            for(std::size_t i = 0; i < key.size(); ++i)
            {
                indices.push_back(i);
            }
            core::String result(key.size(), '*');
            for(std::size_t i = 0; i < level; ++i)
            {
                const auto swapIdx =
                    std::uniform_int_distribution<std::size_t>(
                        i, key.size()-1)(random);
                result[indices[swapIdx]] = key[indices[swapIdx]];
                const auto t = indices[i];
                indices[i] = indices[swapIdx];
                indices[swapIdx] = t;
            }
            render.addText(result);
        }
    }
}
