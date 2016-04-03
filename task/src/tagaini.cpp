#include "task/tagaini.h"

#include <algorithm>
#include <stdexcept>
#include <cctype>

#include "base/strutil.h"
#include "csv/csv.h"

namespace task
{
    namespace tagaini
    {
        core::String trim(const core::String &str)
        {
            core::String::size_type i = 0;
            while(i < str.size() && std::iswspace(str[i]))
                ++i;
            core::String::size_type j = str.size();
            while(j > i && (std::iswspace(str[j-1]) || str[j-1] == L'.'))
                --j;
            return str.substr(i, j-i);
        }

        TaskCollection readCollection(std::wistream &stream)
        {
            TaskCollection result;
            csv::Csv<wchar_t> reader(stream, L'\t', L'\0', L'\\');
            while(true)
            {
                const auto row = reader.row();
                if(!row.second)
                    break;
                if(row.first.size() != 3)
                    throw std::runtime_error("invalid row format");
                auto values = base::strutil::split(
                    row.first[2].begin(), row.first[2].end(), L',');
                std::transform(values.begin(), values.end(), values.begin(), trim);
                result.push_back(std::make_pair(row.first[0], values));
            }
            return result;
        }
    }
}
