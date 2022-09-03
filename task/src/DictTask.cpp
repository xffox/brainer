#include "task/DictTask.h"

#include <algorithm>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <vector>
#include <cwctype>
#include <utility>
#include <cassert>

#include "base/strutil.h"
#include "core/IRender.h"

#include "util.h"

namespace task
{
    namespace
    {
        auto keepAlnum(const core::String &str)
        {
            core::String result;
            std::copy_if(std::begin(str), std::end(str),
                std::back_inserter(result),
                [](auto c){
                    return !(std::iswpunct(c) || std::iswblank(c));
                });
            return result;
        }

        auto normalizeWord(const core::String &str)
        {
            return util::tolower(keepAlnum(str));
        }
    }

    DictTask::DictTask(base::Randomizer &&random,
        const StringCollection &keys, const StringCollection &values)
        :keys(keys), values(values), answers(), random(std::move(random))
    {
        if(values.empty() || keys.empty())
        {
            throw std::runtime_error("task values or keys are empty");
        }
        std::transform(keys.begin(), keys.end(),
            std::inserter(answers, answers.begin()),
            normalizeWord);
    }

    bool DictTask::validateBase(const core::String &result)
    {
        return answers.find(normalizeWord(result)) != answers.end();
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

    void DictTask::hint(core::IRender &render, std::size_t)
    {
        using IdxCollection = std::vector<std::size_t>;
        if(!keys.empty())
        {
            const auto &key = keys.front();
            assert(!key.empty());
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
                    random.uniformInteger<std::size_t>(i, key.size()-1);
                result[indices[swapIdx]] = key[indices[swapIdx]];
                const auto t = indices[i];
                indices[i] = indices[swapIdx];
                indices[swapIdx] = t;
            }
            render.addText(result);
        }
    }
}
