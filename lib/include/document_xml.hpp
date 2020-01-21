#pragma once

#include <memory>
#include "gpds_export.h"
#include "document.hpp"

namespace tinyxml2
{
    class XMLDocument;
}

namespace gpds
{
    class value;

    class GPDS_EXPORT document_xml :
        public document
    {
    public:
        document_xml();
        document_xml(const document_xml& other);
        document_xml(document_xml&& other) = default;
        explicit document_xml(std::unique_ptr<tinyxml2::XMLDocument>&& doc);
        virtual ~document_xml() override;

        [[nodiscard]] virtual std::string to_string() const override;
        [[nodiscard]] virtual std::string query(const std::string& qry) const override;
        [[nodiscard]] virtual value query_value(const std::string& qry) const override;

    private:
        tinyxml2::XMLDocument* m_document;
    };
}