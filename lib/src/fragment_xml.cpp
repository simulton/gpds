#include <algorithm>
#include "fragment_xml.hpp"
#include "3rdparty/tinyxml2/tinyxml2.h"
#include "3rdparty/tinyxml2-ex/tixml2ex.h"
#include "archiver_xml.hpp"

using namespace gpds;


/**
 * @note This is not a member function of @p fragment_xml to prevent exposing the @p tinyxml2::Selector<> type to the public
 *       interface. This way the dependency to tinyxml2 remains internal and clients of this library will not need to worry
 *       about it at all.
 */
static tinyxml2::Selector<tinyxml2::XMLElement> query_elements(const std::string& qry, const fragment_xml::element& element)
{
    // Find the element corresponding to the xpath
    if (std::holds_alternative<tinyxml2::XMLDocument*>(element)) {
        auto doc = std::get<tinyxml2::XMLDocument*>(element);
        // In order to work as expected, the query need to start with a slash
        return tinyxml2::selection(doc->RootElement(), (!qry.empty() && qry[0] == '/') ? qry : '/' + qry);
    }

    return tinyxml2::selection(std::get<tinyxml2::XMLElement*>(element), qry);
}

fragment_xml::fragment_xml(tinyxml2::XMLElement* element) :
    m_element(element)
{
}

fragment_xml::fragment_xml(tinyxml2::XMLDocument* document) :
    m_element(document)
{
}

std::vector<std::string> fragment_xml::query_list(const std::string& qry) const
{
    // Retrieve the elements
    const tinyxml2::Selector<tinyxml2::XMLElement> selection = query_elements(qry, m_element);

    std::vector<std::string> elements;
    for (const auto& el: selection) {
        // Use the printer if it's has child elements
        if (el->FirstChildElement()) {
            tinyxml2::XMLPrinter printer;
            el->Accept(&printer);
            elements.emplace_back(printer.CStr());
        }
        // Convert the element to a string
        elements.emplace_back(tinyxml2::text(el));
    }

    return elements;
}

std::string fragment_xml::query(const std::string& qry) const
{
    std::vector<std::string> elements = query_list(qry);

    if (elements.empty())
        return { };

    return elements.front();
}

std::vector<value> fragment_xml::query_values(const std::string& qry) const
{
    // Retrieve the elements
    const tinyxml2::Selector<tinyxml2::XMLElement> selection = query_elements(qry, m_element);

    std::vector<value> elements;
    for (const auto& el: selection) {
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

        elements.emplace_back(value);
    }

    return elements;
}

value fragment_xml::query_value(const std::string& qry) const
{
    std::vector<value> elements = query_values(qry);

    if (elements.empty())
        return { };

    return elements.front();
}

std::vector<std::unique_ptr<fragment>> fragment_xml::query_fragments(const std::string& qry) const
{
    // Retrieve the elements
    const tinyxml2::Selector<tinyxml2::XMLElement> selection = query_elements(qry, m_element);

    std::vector<std::unique_ptr<fragment>> elements;
    for (const auto& el: selection) {
        // Construct and return a fragment_xml that wraps the element
        elements.emplace_back(std::make_unique<fragment_xml>(el));
    }

    return elements;
}

std::unique_ptr<fragment> fragment_xml::query_fragment(const std::string & qry) const
{
    std::vector<std::unique_ptr<fragment>> elements = query_fragments(qry);

    if (elements.empty())
        return { };

    return std::move(elements.front());
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
