#include <algorithm>
#include "archiver_xml.hpp"
#include "utils.hpp"
#include "3rdparty/tinyxml2/tinyxml2.h"

using namespace gpds;

bool archiver_xml::save(std::ostream& stream, const container& container, const std::string& rootName) const
{
    // Create the document
    tinyxml2::XMLDocument doc;

    // Add the XML declaration
    doc.NewDeclaration();

    // Add the root node
    tinyxml2::XMLElement* root = doc.NewElement(rootName.data());
    doc.InsertEndChild(root);

    // Add the Serialize data
    write_entry(doc, *root, container);

    // Add data to stream
    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);
    stream << printer.CStr();

    // Free up memory
    doc.Clear();

    return true;
}

bool archiver_xml::load(std::istream& stream, container& container, const std::string& rootName)
{
    // Create the document
    std::string string(std::istreambuf_iterator<char>(stream), {});
    tinyxml2::XMLDocument doc;
    doc.Parse(string.data());

    // Retrieve the root node
    tinyxml2::XMLElement* rootNode = doc.FirstChildElement(rootName.data());
    if (!rootNode) {
        return false;
    }

    // Handle all nodes children recursively
    read_entry(*rootNode, container);

    return true;
}

void archiver_xml::write_entry(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement& root, const container& container) const
{
    // Annotate list if supposed to
    if (settings.annotate_list_count && container.is_list()) {
        std::string attributeString = "count";
        if (settings.prefix_annotations) {
            attributeString = NAMESPACE_PREFIX + attributeString;
        }

        root.SetAttribute(attributeString.c_str(), std::to_string(container.values.size()).data());
    }

    // Add all container arguments
    for (const auto& attribute : container.attributes.map) {
        root.SetAttribute(attribute.first.c_str(), attribute.second.c_str());
    }

    // Iterate through all values in this container
    for (const auto& keyValuePair: container.values) {

        // Some aliases to make the code easier to read
        const auto& key = keyValuePair.first.c_str();
        const value& value = keyValuePair.second;

        // Create a new node in the DOM
        tinyxml2::XMLElement* child = nullptr;
        {
            // Nested container
            if (value.is_type<gpds::container*>()) {
                // Recursion
                const auto childContainer = value.get<gpds::container*>().value_or(nullptr);
                if (childContainer) {
                    child = doc.NewElement(key);
                    write_entry(doc, *child, *childContainer);
                }
            }

            // Simple value
            else {
                child = doc.NewElement(key);
                child->SetText(value.get<std::string>().value_or("").data());
            }
        }

        // Add all value arguments
        for (const auto& attribute : value.attributes.map) {
            child->SetAttribute(attribute.first.c_str(), attribute.second.c_str());
        }

        GPDS_ASSERT(child);

        root.InsertEndChild(child);
    }
}

void archiver_xml::read_entry(const tinyxml2::XMLElement& rootNode, container& container)
{
    // Container attributes
    for (const tinyxml2::XMLAttribute* attribute = rootNode.FirstAttribute(); attribute; attribute = attribute->Next())
        container.add_attribute(std::string(attribute->Name()), std::string(attribute->Value()));

    // Handle all nodes children recursively
    for (const tinyxml2::XMLElement* node = rootNode.FirstChildElement(); node; node = node->NextSiblingElement()) {
        // Extract the name & value
        std::string keyString(node->Name());

        // Create the Value
        value value;

        // Value attributes
        for (const tinyxml2::XMLAttribute* attribute = node->FirstAttribute(); attribute; attribute = attribute->Next())
            value.add_attribute(std::string(attribute->Name()), std::string(attribute->Value()));

        // It's a text element or an empty element
        if (node->GetText() || node->NoChildren()) {
            // Get the text if it's a text element
            if (node->GetText()) {
                value.set(std::string(node->GetText()));    // ToDo: construction of temporary std::string unnecessary.
            }
        }

        // It's a another container
        else if (node->FirstChild()) {
            gpds::container childContainer;

            read_entry(*node, childContainer);
            value.set(std::move(childContainer));
        }

        container.add_value(keyString, value);
    }

}
