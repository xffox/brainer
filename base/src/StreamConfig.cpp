#include "base/StreamConfig.h"

#include <string>
#include <cstddef>
#include <istream>
#include <algorithm>
#include <utility>
#include <ctype.h>

#include "base/Nullable.h"

namespace base
{
    namespace
    {
        std::wstring trim(const std::wstring &str)
        {
            if(!str.empty())
            {
                std::wstring::size_type i = 0;
                while(i < str.size() && iswspace(str[i]))
                    ++i;
                std::wstring::size_type j = str.size();
                while(j-1 > i && iswspace(str[j-1]))
                    --j;
                return str.substr(i, j-i);
            }
            return str;
        }

        bool isComment(const std::wstring &line)
        {
            std::wstring::size_type i = 0;
            while(i < line.size() && iswspace(line[i]))
                ++i;
            return (i < line.size() && line[i] == L'#');
        }

        base::Nullable<IConfig::ValuePair> parseLine(const std::wstring &line)
        {
            if (!isComment(line))
            {
                std::wstring::size_type sep = line.find(L'=');
                if(sep != std::wstring::npos)
                {
                    return std::make_pair(trim(line.substr(0, sep)),
                        trim(line.substr(sep+1, std::wstring::npos)));
                }
            }
            return base::Nullable<IConfig::ValuePair>();
        }
    }

    IConfig::ValuesCollection StreamConfig::readStream(std::wistream &stream)
    {
        IConfig::ValuesCollection values;

        const std::size_t BUF_SZ = 4096; 
        wchar_t buf[BUF_SZ];
        std::wstring line;
        do
        {
            stream.read(buf, sizeof(buf)/sizeof(buf[0]));
            const std::streamsize len = stream.gcount();
            if(len > 0)
            {
                wchar_t *begin = &buf[0];
                wchar_t *end = &buf[len];
                while((end = std::find(begin, &buf[len], L'\n')) !=
                    &buf[len])
                {
                    line+=std::wstring(begin, end-begin);
                    base::Nullable<ValuePair> valuePair = parseLine(line);
                    if(!valuePair.isNull())
                        values.push_back(*valuePair);
                    line.clear();
                    begin = end+1;
                }
                line+=std::wstring(begin, end-begin);
            }
        }
        while(stream);
        base::Nullable<ValuePair> valuePair = parseLine(line);
        if(!valuePair.isNull())
            values.push_back(*valuePair);

        return values;
    }
}
