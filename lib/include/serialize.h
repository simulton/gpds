#pragma once

#include "gpds_export.h"
#include "container.h"

namespace Gpds
{

    class GPDS_EXPORT Serialize
    {
    public:
        virtual ~Serialize() = default;

        [[nodiscard]] virtual Gpds::Container toContainer() const = 0;
        virtual void fromContainer(const Gpds::Container& container) = 0;
    };

}
