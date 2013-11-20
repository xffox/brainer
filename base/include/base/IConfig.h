#ifndef BASE_ICONFIG_H
#define BASE_ICONFIG_H

#include <string>
#include <map>

namespace base
{
    class IConfig
    {
    public:
        typedef std::map<std::string, std::string> ValuesMap;

    public:
        virtual ~IConfig(){}

        // throw std::exception on error
        virtual ValuesMap read() = 0;
    };
}

#endif
