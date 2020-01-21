#pragma once

#include "3rdparty/tinyxml2/tinyxml2.h"
#include "3rdparty/tinyxml2-ex/tixml2ex.h"

namespace gpds
{
    class GPDS_EXPORT document
    {
    public:
        [[nodiscard]] virtual std::string query(const std::string& qry) const = 0;
        [[nodiscard]] virtual value query_value(const std::string& qry) const = 0;
    };
}