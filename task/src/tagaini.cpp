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
        TaskCollection readCollection(std::wistream &stream)
        {
            struct Trimmer
            {
                bool operator()(core::String::value_type ch) const
                {
                    return std::iswspace(ch) || ch == L'.';
                }
            };
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
                std::transform(values.begin(), values.end(), values.begin(),
                    [](const decltype(values)::value_type &str) {
                        return base::strutil::trim(str, Trimmer());
                    });
                auto prons = base::strutil::split(
                    row.first[1].begin(), row.first[1].end(), L',');
                result.push_back(std::make_tuple(row.first[0], prons, values));
            }
            return result;
        }
    }
}
