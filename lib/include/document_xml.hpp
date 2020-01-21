#pragma once

#include "document.hpp"
#include "3rdparty/tinyxml2/tinyxml2.h"
#include "3rdparty/tinyxml2-ex/tixml2ex.h"

namespace gpds
{
    class GPDS_EXPORT document_xml : public document
    {
    public:
        explicit document_xml(tinyxml2::XMLDocument* doc) :
            m_document(doc)
        {
        }

        [[nodiscard]] value query(const std::string& qry) const override
        {
            tinyxml2::XMLElement* el = tinyxml2::find_element(*m_document, qry);
            value val;
            val.from_string(tinyxml2::text(el));
            return val;
        }
    private:
        tinyxml2::XMLDocument* m_document;
    };
}