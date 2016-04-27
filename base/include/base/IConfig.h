#ifndef BASE_ICONFIG_H
#define BASE_ICONFIG_H

#include <string>
#include <vector>

namespace base
{
    class IConfig
    {
    public:
        typedef std::pair<std::wstring, std::wstring> ValuePair;
        typedef std::vector<ValuePair> ValuesCollection;

    public:
        virtual ~IConfig(){}

        // throw std::exception on error
        virtual ValuesCollection read() = 0;
    };
}

#endif
