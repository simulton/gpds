#include <algorithm>
#include "fragment_xml.hpp"
#include "3rdparty/tinyxml2/tinyxml2.h"
#include "3rdparty/tinyxml2-ex/tixml2ex.h"
#include "archiver_xml.hpp"

using namespace gpds;

fragment_xml::fragment_xml(tinyxml2::XMLElement* element) :
    m_element(element)
{
}

fragment_xml::fragment_xml(tinyxml2::XMLDocument* document) :
    m_element(document)
{
}

std::string fragment_xml::query(const std::string& qry) const
{
    // Retrieve the element
    const tinyxml2::XMLElement* el = query_element(qry);

    // Return empty string if there is no element
    if (not el)
        return { };

    // Use the printer if it's has child elements
    if (el->FirstChildElement()) {
        tinyxml2::XMLPrinter printer;
        el->Accept(&printer);
        return printer.CStr();
    }

    // Convert the element to a string
    return tinyxml2::text(el);
}

gpds::container fragment_xml::to_container() const
{
    gpds::container container;
    archiver_xml ar;

    // If it's a document create a wrapper around the root container
    if (std::holds_alternative<tinyxml2::XMLDocument*>(m_element)) {
        auto document = std::get<tinyxml2::XMLDocument*>(m_element);
        gpds::container rootContainer;
        ar.read_entry(*document->RootElement(), rootContainer);
        container.add_value(document->RootElement()->Name(), rootContainer);
    } else {
        auto element = std::get<tinyxml2::XMLElement*>(m_element);
        ar.read_entry(*element, container);
    }

    return container;
}

value fragment_xml::query_value(const std::string& qry) const
{
    // Retrieve the element
    tinyxml2::XMLElement* el = query_element(qry);

    // Return an empty value if there is no element
    if (not el)
        return { };


    // Convert the element to a value
    value value;
    if (el->FirstChildElement()) {
        // Take special care if it's a container
        archiver_xml ar;
        auto container = new gpds::container;
        ar.read_entry(*el, *container);
        value.set(container);
    } else {
        value.from_string(tinyxml2::text(el));
    }

    return value;
}

std::unique_ptr<fragment> fragment_xml::query_fragment(const std::string& qry) const
{
    // Retrieve the element
    tinyxml2::XMLElement* el = query_element(qry);

    // Return nullptr if there is no element
    if (not el) {
        return nullptr;
    }

    // Construct and return a fragment_xml that wraps the element
    return std::make_unique<fragment_xml>(el);
}

tinyxml2::XMLElement* fragment_xml::query_element(const std::string& qry) const
{
    tinyxml2::XMLElement* el;

    // Find the element corresponding to the xpath
    if (std::holds_alternative<tinyxml2::XMLDocument*>(m_element)) {
        auto doc = std::get<tinyxml2::XMLDocument*>(m_element);
        // In order to work as expected, the query need to start with a slash
        el = tinyxml2::find_element(doc->RootElement(), (!qry.empty() && qry[0] == '/') ? qry : '/' + qry);
    } else {
        auto element = std::get<tinyxml2::XMLElement*>(m_element);
        el = tinyxml2::find_element(element, qry);
    }

    return el;
}
