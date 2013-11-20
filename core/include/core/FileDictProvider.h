#ifndef CORE_FILEDICTPROVIDER_H
#define CORE_FILEDICTPROVIDER_H

#include <map>

#include "IDictProvider.h"

namespace core
{
    class FileDictProvider: public IDictProvider
    {
    public:
        FileDictProvider(const std::string &filename);
        virtual ~FileDictProvider();

        virtual std::string get(const std::string &key) const;
        virtual void set(const std::string &key, const std::string &value);

        virtual std::auto_ptr<IIterator<std::string> > keys() const;
        
    protected:
        typedef std::map<std::string, std::string> ValueMap;

    protected:
        void readValues();
        void writeValues();

    private:
        class Iterator;

    private:
        std::string filename;
        ValueMap values;
    };
}

#endif
