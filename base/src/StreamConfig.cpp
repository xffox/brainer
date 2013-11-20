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
        typedef std::pair<std::string, std::string> ValuePair;

        std::string trim(const std::string &str)
        {
            if(!str.empty())
            {
                std::string::size_type i = 0;
                while(i < str.size() && isspace(str[i]))
                    ++i;
                std::string::size_type j = str.size();
                while(j-1 > i && isspace(str[j-1]))
                    --j;
                return str.substr(i, j-i);
            }
            return str;
        }

        bool isComment(const std::string &line)
        {
            std::string::size_type i = 0;
            while(i < line.size() && isspace(line[i]))
                ++i;
            return (i < line.size() && line[i] == '#');
        }

        base::Nullable<ValuePair> parseLine(const std::string &line)
        {
            if (!isComment(line))
            {
                std::string::size_type sep = line.find('=');
                if(sep != std::string::npos)
                {
                    return std::make_pair(trim(line.substr(0, sep)),
                        trim(line.substr(sep+1, std::string::npos)));
                }
            }
            return base::Nullable<ValuePair>();
        }
    }

    IConfig::ValuesMap StreamConfig::readStream(std::istream &stream)
    {
        IConfig::ValuesMap values;

        const std::size_t BUF_SZ = 4096; 
        char buf[BUF_SZ];
        std::string line;
        do
        {
            stream.read(buf, sizeof(buf));
            const std::streamsize len = stream.gcount();
            if(len > 0)
            {
                char *begin = &buf[0];
                char *end = 0;
                while((end = std::find(begin, &buf[len], '\n')) !=
                    &buf[len])
                {
                    line+=std::string(begin, end-begin);
                    base::Nullable<ValuePair> valuePair = parseLine(line);
                    if(!valuePair.isNull())
                        values.insert(*valuePair);
                    line.clear();
                    begin = end+1;
                }
                line+=std::string(begin, end-begin);
            }
        }
        while(stream);
        base::Nullable<ValuePair> valuePair = parseLine(line);
        if(!valuePair.isNull())
            values.insert(*valuePair);

        return values;
    }
}
