#pragma once

#include <ostream>
#include <istream>
#include "archiver.hpp"

namespace tinyxml2
{
    class XMLDocument;
    class XMLElement;
}

namespace gpds
{
    class GPDS_EXPORT archiver_xml : public archiver
    {
    public:
        // Deal with name hiding
        using archiver::save;
        using archiver::load;

        const std::string NAMESPACE_PREFIX = "gpds:";

        struct settings
        {
            bool print_comments;
            bool annotate_list_count;
            bool annotate_types;
            bool prefix_annotations;
        };

        settings settings;

        archiver_xml();
        archiver_xml(const archiver_xml& other) = default;
        archiver_xml(archiver_xml&& other) = default;
        virtual ~archiver_xml() override = default;

        virtual bool save(std::ostream& stream, const container& container, const std::string& rootName) const override;
        virtual bool load(std::istream& stream, container& container, const std::string& rootName) override;

    private:
        void write_entry(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement& root, const container& container) const;
        void read_entry(tinyxml2::XMLElement& rootNode, container& container);
    };

}
