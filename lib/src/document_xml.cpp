#include <vector>
#include <algorithm>
#include "document_xml.hpp"
#include "archiver_xml.hpp"
#include "3rdparty/tinyxml2/tinyxml2.h"

using namespace gpds;

document_xml::document_xml() :
    m_root(nullptr)
{
}

document_xml::document_xml(const document_xml& other) :
    m_root(nullptr)
{
    if (other.m_root) {
        auto doc = std::get<tinyxml2::XMLDocument*>(other.m_root->m_element);
        if (doc) {
            auto xmlDoc = new tinyxml2::XMLDocument;
            doc->DeepCopy(xmlDoc);
            m_root = std::make_unique<fragment_xml>(xmlDoc);
        }
    }
}

document_xml::document_xml(std::unique_ptr<tinyxml2::XMLDocument>&& doc)
{
    m_root = std::make_unique<fragment_xml>(doc.release());
    doc.reset();
}

std::string document_xml::to_string() const
{
    if (not m_root)
        return { };

    tinyxml2::XMLPrinter printer;
    auto document = std::get<tinyxml2::XMLDocument*>(m_root->m_element);

    if (not document)
        return { };

    document->Print(&printer);

    return printer.CStr();
}

std::vector<std::string> document_xml::query_list(const std::string& qry) const
{
    if (not m_root)
        return { };

    return m_root->query_list(qry);
}

std::string document_xml::query(const std::string& qry) const
{
    if (not m_root)
        return { };

    return m_root->query(qry);
}

std::vector<value> document_xml::query_values(const std::string& qry) const
{
    if (not m_root)
        return { };

    return m_root->query_values(qry);
}

value document_xml::query_value(const std::string& qry) const
{
    if (not m_root)
        return { };

    return m_root->query_value(qry);
}

std::vector<std::unique_ptr<fragment>> document_xml::query_fragments(const std::string& qry) const
{
    if (not m_root)
        return { };

    return m_root->query_fragments(qry);
}

std::unique_ptr<fragment> document_xml::query_fragment(const std::string& qry) const
{
    if (not m_root)
        return { };

    return m_root->query_fragment(qry);
}
