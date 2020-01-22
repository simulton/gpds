#include <vector>
#include <algorithm>
#include "3rdparty/tinyxml2-ex/tixml2ex.h"
#include "document_xml.hpp"
#include "archiver_xml.hpp"

using namespace gpds;

document_xml::document_xml() :
    m_document(nullptr)
{
}

document_xml::document_xml(const document_xml& other) :
    m_document(nullptr)
{
    if (other.m_document) {
        m_document = new tinyxml2::XMLDocument;
        other.m_document->DeepCopy(m_document);
    }
}

document_xml::document_xml(std::unique_ptr<tinyxml2::XMLDocument>&& doc) :
    m_document(doc.release())
{
    doc.reset();
}

document_xml::~document_xml()
{
    if (m_document)
        delete m_document;
}

std::string document_xml::to_string() const
{
    if (not m_document)
        return { };

    tinyxml2::XMLPrinter printer;
    m_document->Print(&printer);

    return printer.CStr();
}

std::string document_xml::query(const std::string& qry) const
{
    if (not m_document)
        return { };

    tinyxml2::XMLElement* el = tinyxml2::find_element(*m_document, qry);

    if (not el)
        return { };

    // Take special care if it's an XML fragment
    if (el->FirstChildElement()) {
        tinyxml2::XMLPrinter printer;
        el->Accept(&printer);
        return printer.CStr();
    }

    return tinyxml2::text(el);
}

[[nodiscard]] value document_xml::query_value(const std::string& qry) const
{
    if (not m_document)
        return { };

    tinyxml2::XMLElement* el = tinyxml2::find_element(*m_document, qry);

    if (not el)
        return { };

    value value;

    // It's a container
    if (el->FirstChildElement()) {
        archiver_xml ar;
        auto container = new gpds::container;
        ar.read_entry(*el, *container);
        value.set(container);
    } else {
        value.from_string(tinyxml2::text(el));
    }

    return value;
}
