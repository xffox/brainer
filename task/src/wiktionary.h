#ifndef TASK_WIKTIONARY_H
#define TASK_WIKTIONARY_H

#include <string>
#include <istream>
#include <vector>
#include <utility>

#include "core/String.h"
#include "task/StringCollection.h"

namespace task
{
    namespace wiktionary
    {
        using TaskCollection = std::vector<std::pair<StringCollection, StringCollection>>;

        TaskCollection readWiktionaryDefinitions(std::wistream &stream);
    }
}

#endif
