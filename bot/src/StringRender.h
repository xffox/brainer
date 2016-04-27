#ifndef BOT_STRINGRENDER_H
#define BOT_STRINGRENDER_H

#include "core/IRender.h"
#include "core/String.h"

namespace bot
{
    class StringRender: public core::IRender
    {
    public:
        virtual void addText(const core::String &str);

        const core::String &text() const;

    private:
        core::String result;
    };
}

#endif
