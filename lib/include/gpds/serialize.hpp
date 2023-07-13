#pragma once

#include "container.hpp"

#include <filesystem>

namespace gpds
{

    /**
     * An interface for (de)serializable objects.
     */
    class serialize
    {
    public:
        /**
         * The mode of the serialize.
         */
        // ToDo: This is a bit whacky. We should ensure that all serializing interfaces accept the archiver as a template parameter instead.
        enum class mode
        {
            XML,
            YAML,
        };

        virtual
        ~serialize() = default;

        // Container
        [[nodiscard]] virtual gpds::container to_container() const = 0;
        virtual void from_container(const gpds::container& container) = 0;

        // String
        std::pair<bool, std::string> to_string(std::string& str, std::string_view root_name, enum mode mode = mode::XML) const;
        std::pair<bool, std::string> from_string(std::string_view str, std::string_view root_name, enum mode mode = mode::XML);

        // File
        std::pair<bool, std::string> to_file(const std::filesystem::path& path, std::string_view root_name, enum mode mode = mode::XML) const;
        std::pair<bool, std::string> from_file(const std::filesystem::path& path, std::string_view root_name, enum mode mode = mode::XML);
    };

}
