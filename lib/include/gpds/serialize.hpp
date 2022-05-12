#pragma once

#include <filesystem>
#include "gpds_export.hpp"
#include "container.hpp"

namespace gpds
{

    /**
     * An interface for (de)serializable objects.
     */
    class GPDS_EXPORT serialize
    {
    public:
        virtual ~serialize() = default;

        // Container
        [[nodiscard]] virtual gpds::container to_container() const = 0;
        virtual void from_container(const gpds::container& container) = 0;

        // String
        std::pair<bool, std::string> to_string(std::string& str, std::string_view root_name) const;
        std::pair<bool, std::string> from_string(std::string_view str, std::string_view root_name);

        // File
        std::pair<bool, std::string> to_file(const std::filesystem::path& path, std::string_view root_name) const;
        std::pair<bool, std::string> from_file(const std::filesystem::path& path, std::string_view root_name);
    };

}
