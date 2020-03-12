#pragma once

#include <filesystem>
#include "gpds_export.h"
#include "container.hpp"

namespace gpds
{

    class GPDS_EXPORT serialize
    {
    public:
        virtual ~serialize() = default;

        // Container
        [[nodiscard]] virtual gpds::container to_container() const = 0;
        virtual void from_container(const gpds::container& container) = 0;

        // File
        std::pair<bool, std::string> to_file(const std::filesystem::path& path, const std::string& root_name) const;
        std::pair<bool, std::string> from_file(const std::filesystem::path& path, const std::string& root_name);
    };

}
