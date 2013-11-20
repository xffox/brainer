#ifndef CORE_IDICTPROVIDER_H
#define CORE_IDICTPROVIDER_H

#include <cstddef>
#include <memory>
#include <string>

namespace core
{
    template<typename T>
    class IIterator;

    class IDictProvider
    {
    public:
        virtual ~IDictProvider(){}

        // \throw exception on not found
        virtual std::string get(const std::string &key) const = 0;
        virtual void set(const std::string &key, const std::string &value) = 0;

        virtual std::auto_ptr<IIterator<std::string> > keys() const = 0;
    };
}

#endif
