#pragma once

#include <ostream>
#include <istream>
#include <gpds/archiver.h>
#include "3rdparty/rapidxml-1.13/rapidxml.hpp"

namespace Gpds
{
    class ArchiverXml : public Archiver
    {
    public:
        // Deal with name hiding
        using Archiver::save;
        using Archiver::load;

        const std::string NAMESPACE_PREFIX = "gpds:";

        struct Settings {
            bool printComments;
            bool annotateListCount;
            bool annotateTypes;
            bool prefixAnnotations;
        };

        Settings settings;

        ArchiverXml();
        ArchiverXml( const ArchiverXml& other ) = default;
        ArchiverXml( ArchiverXml&& other ) = default;
        virtual ~ArchiverXml() override = default;

        virtual bool save(std::ostream& stream, const Container& container, const std::string& rootName) const override;
        virtual bool load(std::istream& stream, Container& container, const std::string& rootName) override;

    private:
        void writeEntry(rapidxml::xml_document<>& doc, rapidxml::xml_node<>& root, const Container& container) const;
        void readEntry(rapidxml::xml_node<>& rootNode, Container& container);
    };

}
