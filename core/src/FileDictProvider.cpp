#include "core/FileDictProvider.h"

#include <algorithm>
#include <exception>
#include <functional>

#include "base/FileConfig.h"
#include "core/IIterator.h"

namespace core
{
    class FileDictProvider::Iterator: public IIterator<std::string>
    {
    public:
        Iterator(ValueMap::const_iterator iter,
            ValueMap::const_iterator endIter)
            :first(true), iter(iter), endIter(endIter)
        {}

        virtual const std::string *next();
        
    private:
        bool first;
        ValueMap::const_iterator iter;
        const ValueMap::const_iterator endIter;
    };

    FileDictProvider::FileDictProvider(const std::string &filename)
        :filename(filename), values()
    {
        readValues();
    }

    FileDictProvider::~FileDictProvider()
    {
    }

    std::string FileDictProvider::get(const std::string &key) const
    {
        const ValueMap::const_iterator iter = values.find(key);
        if(iter != values.end())
            return iter->second;
        throw std::exception();
    }

    void FileDictProvider::set(const std::string &key,
        const std::string &value)
    {
        values[key] = value;
    }

    std::auto_ptr<IIterator<std::string> > FileDictProvider::keys() const
    {
        return std::auto_ptr<IIterator<std::string> >(
            new Iterator(values.begin(), values.end()));
    }

    void FileDictProvider::readValues()
    {
        try
        {
            base::FileConfig conf(filename);
            values = conf.read();
        }
        catch(const std::exception&)
        {
        }
    }

    void FileDictProvider::writeValues()
    {
    }

    const std::string *FileDictProvider::Iterator::next()
    {
        if(!first)
            ++iter;
        else
            first = false;
        if(iter != endIter)
            return &iter->first;
        else
            return 0;
    }
}
