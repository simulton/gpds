#pragma once

#include <ostream>
#include "archiver.hpp"

#include <istream>

namespace tinyxml2
{
    class XMLDocument;
    class XMLElement;
}

namespace gpds
{
    /**
     * An archiver to (de)serialize to/from XML.
     */
    class GPDS_EXPORT archiver_xml :
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

        archiver_xml() = default;
        archiver_xml(const archiver_xml& other) = delete;
        archiver_xml(archiver_xml&& other) noexcept = delete;
        ~archiver_xml() override = default;

        archiver_xml& operator=(const archiver_xml& rhs) = delete;
        archiver_xml& operator=(archiver_xml&& rhs) noexcept = delete;

        bool save(std::ostream& stream, const container& container, const std::string& rootName) const override;
        bool load(std::istream& stream, container& container, const std::string& rootName) override;

    private:
        void write_entry(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement& root, const container& container) const;
        void read_entry(const tinyxml2::XMLElement& rootNode, container& container);
    };

}
