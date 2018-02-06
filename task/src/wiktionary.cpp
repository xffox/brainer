#include "wiktionary.h"

#include <cstddef>
#include <stdexcept>

#include "csv/csv.h"

namespace task
{
    namespace wiktionary
    {
        namespace
        {
            constexpr std::size_t COLUMNS = 2;
        }

        TaskCollection readWiktionaryDefinitions(std::wistream &stream)
        {
            TaskCollection result;
            csv::Csv<wchar_t> reader(stream, L',', L'"', L'\0');
            while(true)
            {
                auto row = reader.row();
                if(!row.second)
                    break;
                if(row.first.size() != COLUMNS)
                    throw std::runtime_error("invalid row format");
                result.emplace_back(
                    StringCollection{std::move(row.first[0])},
                    StringCollection{std::move(row.first[1])});
            }
            return result;
        }
    }
}
