#pragma once

#include <gpds/container.h>

namespace Gpds
{

    class Serialize
    {
    public:
        virtual ~Serialize() = default;

        virtual Gpds::Container toContainer() const = 0;
        virtual void fromContainer(const Gpds::Container& container) = 0;
    };

}
