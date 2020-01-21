#include <vector>
#include <algorithm>
#include "3rdparty/tinyxml2/tinyxml2.h"
#include "3rdparty/tinyxml2-ex/tixml2ex.h"
#include "document_xml.hpp"
#include "archiver_xml.hpp"

using namespace gpds;

document_xml::document_xml(std::unique_ptr<tinyxml2::XMLDocument>&& doc) :
    m_document(std::move(doc))
{
}

std::string document_xml::query(const std::string& qry) const
{
    tinyxml2::XMLElement* el = tinyxml2::find_element(*m_document, qry);
    return tinyxml2::text(el);
}

[[nodiscard]] value document_xml::query_value(const std::string& qry) const
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
