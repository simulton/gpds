#pragma once

#include "container.h"

namespace Gds
{

    class Serialize
    {
    public:
        virtual ~Serialize() = default;

        virtual Container toContainer() const = 0;
        virtual void fromContainer(const Container& container) = 0;
    };

}
