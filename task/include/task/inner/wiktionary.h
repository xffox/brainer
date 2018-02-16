#ifndef TASK_INNER_WIKTIONARY_H
#define TASK_INNER_WIKTIONARY_H

#include <string>
#include <istream>
#include <vector>
#include <utility>

#include "core/String.h"
#include "task/StringCollection.h"

namespace task
{
namespace inner
{
namespace wiktionary
{
    using TaskCollection =
        std::vector<std::pair<StringCollection, StringCollection>>;

    TaskCollection readWiktionaryDefinitions(std::wistream &stream);
    core::String prepareDescription(const core::String &descr,
        const core::String &term);
    bool suitableTerm(const core::String &term);
}
}
}

#endif
