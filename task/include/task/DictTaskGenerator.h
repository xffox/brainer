#ifndef TASK_DICTTASKGENERATOR_H
#define TASK_DICTTASKGENERATOR_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "core/ITaskGenerator.h"

namespace core
{
    class IDictProvider;
}

namespace task
{
    class DictTaskGenerator: public core::ITaskGenerator
    {
    public:
        typedef std::map<std::string, std::string> TaskMap;

    public:
        DictTaskGenerator(unsigned int seed,
            std::auto_ptr<core::IDictProvider> provider);
        virtual ~DictTaskGenerator();

        virtual std::auto_ptr<core::ITask> generateTask();

    private:
        typedef std::vector<std::string> StringCollection;

    private:
        void readKeys();

    private:
        StringCollection keys;
        std::auto_ptr<core::IDictProvider> provider;
    };
}

#endif
