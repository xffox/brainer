#include "task/DictTaskGenerator.h"

#include <cassert>
#include <exception>
#include <memory>
#include <cstdlib>
#include <iterator>

#include "task/DictTask.h"
#include "core/IDictProvider.h"
#include "core/IIterator.h"

namespace task
{
    DictTaskGenerator::DictTaskGenerator(unsigned int seed,
        std::auto_ptr<core::IDictProvider> provider)
        :keys(), provider(provider)
    {
        if(!this->provider.get())
            // TODO: exception
            throw std::exception();
        srand(seed);
        readKeys();
    }

    DictTaskGenerator::~DictTaskGenerator()
    {
    }

    void DictTaskGenerator::readKeys()
    {
        assert(provider.get());
        std::auto_ptr<core::IIterator<std::string> > iter = provider->keys();
        assert(iter.get());
        const std::string *k = 0;
        keys.clear();
        while((k = iter->next()))
            keys.push_back(*k);
    }

    std::auto_ptr<core::ITask> DictTaskGenerator::generateTask()
    {
        if(!keys.empty())
        {
            assert(provider.get());
            std::size_t pos = rand()%keys.size();
            const std::string &k = keys[pos];
            try
            {
                return std::auto_ptr<core::ITask>(new DictTask(k,
                        provider->get(k)));
            }
            catch(const std::exception&)
            {
            }
        }
        // TODO: exception
        return std::auto_ptr<core::ITask>();
    }
}
