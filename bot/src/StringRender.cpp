#include "StringRender.h"

namespace bot
{
    void StringRender::addText(const core::String &str)
    {
        this->result += str;
    }

    const core::String &StringRender::text() const
    {
        return result;
    }
}
