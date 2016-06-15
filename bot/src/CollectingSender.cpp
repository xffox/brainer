#include "CollectingSender.h"

namespace bot
{
    void CollectingSender::send(const std::string &msg)
    {
        messages.push_back(msg);
    }
}
