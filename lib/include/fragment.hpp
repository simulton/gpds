#pragma once

#include <string>
#include <memory>
#include "value.hpp"
#include "gpds_export.hpp"

namespace gpds
{
    class container;

    class GPDS_EXPORT fragment
    {
    public:
        [[nodiscard]] virtual std::vector<std::string> query_list(const std::string& qry) const = 0;
        [[nodiscard]] virtual std::string query(const std::string& qry) const = 0;
        [[nodiscard]] virtual std::vector<value> query_values(const std::string& qry) const = 0;
        [[nodiscard]] virtual value query_value(const std::string& qry) const = 0;
        [[nodiscard]] virtual std::vector<std::unique_ptr<fragment>> query_fragments(const std::string& qry) const = 0;
        [[nodiscard]] virtual std::unique_ptr<fragment> query_fragment(const std::string& qry) const = 0;
        [[nodiscard]] virtual container to_container() const = 0;
    };
}



