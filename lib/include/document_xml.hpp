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

        [[nodiscard]] std::string query(const std::string& qry) const override
        {
            tinyxml2::XMLElement* el = tinyxml2::find_element(*m_document, qry);
            return tinyxml2::text(el);
        }

        [[nodiscard]] value query_value(const std::string& qry) const override
        {
            tinyxml2::XMLElement* el = tinyxml2::find_element(*m_document, qry);
            value value;
            // It's a container
            if (el->FirstChild()) {
                archiver_xml ar;
                auto container = new gpds::container;
                ar.read_entry(*el, *container);
                value.set(container);
            } else {
                value.from_string(tinyxml2::text(el));
            }
            return value;
        }
    private:
        tinyxml2::XMLDocument* m_document;
    };
}