#pragma once

#include <string>
#include "fragment.hpp"

namespace gpds
{
    class value;

    class GPDS_EXPORT document
    {
    public:
        virtual ~document() = default;

        [[nodiscard]] virtual std::string to_string() const = 0;
        [[nodiscard]] virtual std::vector<std::string> query_list(const std::string& qry) const = 0;
        [[nodiscard]] virtual std::string query(const std::string& qry) const = 0;
        [[nodiscard]] virtual std::vector<value> query_values(const std::string& qry) const = 0;
        [[nodiscard]] virtual value query_value(const std::string& qry) const = 0;
        [[nodiscard]] virtual std::vector<std::unique_ptr<fragment>> query_fragments(const std::string& qry) const = 0;
        [[nodiscard]] virtual std::unique_ptr<fragment> query_fragment(const std::string& qry) const = 0;
    };
}
