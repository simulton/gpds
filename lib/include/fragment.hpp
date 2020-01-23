#pragma once

#include <string>
#include <memory>
#include "value.hpp"
#include "gpds_export.h"

namespace gpds
{
    class container;

    class GPDS_EXPORT fragment
    {
    public:
        [[nodiscard]] virtual std::string query(const std::string& qry) const = 0;
        [[nodiscard]] virtual value query_value(const std::string& qry) const = 0;
        [[nodiscard]] virtual std::unique_ptr<fragment> query_fragment(const std::string& qry) const = 0;
        [[nodiscard]] virtual container to_container() const = 0;
    };
}



