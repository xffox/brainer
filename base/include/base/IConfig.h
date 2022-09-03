#ifndef BASE_ICONFIG_H
#define BASE_ICONFIG_H

#include <string>
#include <vector>

namespace base
{
    class IConfig
    {
    public:
        using ValuePair = std::pair<std::wstring, std::wstring> ;
        using ValuesCollection = std::vector<ValuePair> ;

    public:
        virtual ~IConfig() = default;

        // throw std::exception on error
        virtual ValuesCollection read() = 0;
    };
}

#endif
