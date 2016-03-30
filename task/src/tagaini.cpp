#include "task/tagaini.h"

#include <stdexcept>

#include "csv/csv.h"

namespace task
{
    namespace tagaini
    {
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
                result.push_back(std::make_pair(row.first[2], row.first[0]));
            }
            return result;
        }
    }
}
