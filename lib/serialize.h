#pragma once

#include "container.h"

namespace Gds
{

    class Serialize
    {
    public:
        virtual ~Serialize() = default;

        virtual Gds::Container toContainer() const = 0;
        virtual void fromContainer(const Gds::Container& container) = 0;
    };

}
