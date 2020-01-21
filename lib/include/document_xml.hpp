#pragma once

#include "gpds_export.h"
#include "document.hpp"

namespace gpds
{
    class value;

    class GPDS_EXPORT document_xml :
        public document
    {
    public:
        explicit document_xml(std::unique_ptr<tinyxml2::XMLDocument>&& doc);

        [[nodiscard]] virtual std::string query(const std::string& qry) const override;
        [[nodiscard]] virtual value query_value(const std::string& qry) const override;

    private:
        std::unique_ptr<tinyxml2::XMLDocument> m_document;
    };
}