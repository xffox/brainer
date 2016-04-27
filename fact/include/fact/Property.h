#ifndef FACT_PROPERTY_H
#define FACT_PROPERTY_H

#include "fact/PropertyId.h"
#include "fact/Variant.h"

namespace fact
{
    struct Property
    {
        PropertyId propertyId;
        Variant value;
    };
}

#endif
