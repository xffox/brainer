#ifndef CORE_ITASKPROVIDER_H
#define CORE_ITASKPROVIDER_H

#include <set>
#include <string>
#include <memory>

namespace core
{
    class ITaskGenerator;

    class ITaskProvider
    {
    public:
        typedef std::set<std::string> StringSet;

    public:
        virtual ~ITaskProvider(){}

        virtual StringSet getTasks() const = 0;
        // throw exception
        virtual std::unique_ptr<ITaskGenerator> create(
            const std::string &name) = 0;
    };
}

#endif
