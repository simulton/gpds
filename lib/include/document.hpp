#pragma once

#include <string>

namespace gpds
{
    class value;

    class GPDS_EXPORT document
    {
    public:
        [[nodiscard]] virtual std::string query(const std::string& qry) const = 0;
        [[nodiscard]] virtual value query_value(const std::string& qry) const = 0;
    };
}
