#ifndef BASE_STRINGVALUE_H
#define BASE_STRINGVALUE_H

#include <string>

#include "StringValueTraits.h"

namespace base
{
    class StringValue
    {
    public:
        StringValue(const std::string &str)
            :strValue(str)
        {}

        operator const std::string&() const {
            return str();
        }

        const std::string &str() const {
            return strValue;
        }

        template<typename T>
        bool extract(T &value) const;

    private:
        std::string strValue;
    };

    template<typename T>
    bool StringValue::extract(T &value) const
    {
        return stringvalue::StringValueTraits<T>::from(value, strValue);
    }
}

#endif
