#pragma once

#include "archiver.hpp"

#include <istream>
#include <ostream>

namespace Yaml
{
    class Node;
}

namespace gpds
{
    /**
     * An archiver to (de)serialize to/from YAML.
     */
    class archiver_yaml :
        public archiver
    {
    public:
        // Deal with name hiding
        using archiver::save;
        using archiver::load;

        const std::string NAMESPACE_PREFIX = "gpds:";

        struct settings
        {
            bool annotate_list_count = false;
            bool prefix_annotations  = true;
        };

        struct settings settings;

        archiver_yaml() = default;
        archiver_yaml(const archiver_yaml& other) = delete;
        archiver_yaml(archiver_yaml&& other) noexcept = delete;
        ~archiver_yaml() override = default;

        archiver_yaml& operator=(const archiver_yaml& rhs) = delete;
        archiver_yaml& operator=(archiver_yaml&& rhs) noexcept = delete;

        bool save(std::ostream& stream, const container& container, std::string_view root_name) const override;
        bool load(std::istream& stream, container& container, std::string_view root_name) override;

    private:
        void write_entry(Yaml::Node& root, const container& container) const;
        void read_entry(const Yaml::Node& root, container& container);
        bool key_exist(const Yaml::Node& root, const std::string& key) const;
        void read_value(const Yaml::Node& node, value& value) const;
    };

}
