#pragma once

#include <filesystem>
#include "container.hpp"

namespace gpds
{

    /**
     * An interface for (de)serializable objects.
     */
    class serialize
    {
    public:
        virtual ~serialize() = default;

        // Container
        [[nodiscard]] virtual gpds::container to_container() const = 0;
        virtual void from_container(const gpds::container& container) = 0;

        // String
        std::pair<bool, std::string> to_string(std::string& str, const std::string& root_name) const;
        std::pair<bool, std::string> from_string(const std::string_view& str, const std::string& root_name);

        // File
        std::pair<bool, std::string> to_file(const std::filesystem::path& path, const std::string& root_name) const;
        std::pair<bool, std::string> from_file(const std::filesystem::path& path, const std::string& root_name);
    };

}
