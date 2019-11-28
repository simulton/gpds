#pragma once

#include "gpds_export.h"
#include "container.hpp"

namespace gpds
{

    class GPDS_EXPORT serialize
    {
    public:
        virtual ~serialize() = default;

        [[nodiscard]] virtual gpds::container to_container() const = 0;
        virtual void from_container(const gpds::container& container) = 0;
    };

}
