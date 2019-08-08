#include <algorithm>
#include "archiverxml.h"
#include "utils.h"
#include "3rdparty/tinyxml2/tinyxml2.h"

using namespace Gpds;

ArchiverXml::ArchiverXml()
{
    settings.printComments = true;
    settings.annotateListCount = false;
    settings.annotateTypes = false;
    settings.prefixAnnotations = true;
}

bool ArchiverXml::save(std::ostream& stream, const Container& container, const std::string& rootName) const
{
    // Create the document
    tinyxml2::XMLDocument doc;

    // Add the XML declaration
    doc.NewDeclaration();

    // Add the root node
    tinyxml2::XMLElement* root = doc.NewElement(rootName.data());
    doc.InsertEndChild(root);

    // Add the Serialize data
    writeEntry(doc, *root, container);

    // Add data to stream
    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);
    stream << printer.CStr();

    // Free up memory
    doc.Clear();

    return true;
}

bool ArchiverXml::load(std::istream& stream, Container& container, const std::string& rootName)
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
    readEntry(*rootNode, container);

    return true;
}

void ArchiverXml::writeEntry(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement& root, const Container& container) const
{
    // Annotate list if supposed to
    if (settings.annotateListCount and container.isList()) {
        std::string attributeString = "count";
        if (settings.prefixAnnotations) {
            attributeString = NAMESPACE_PREFIX + attributeString;
        }

        root.SetAttribute(attributeString.c_str(), std::to_string(container.values.size()).data());
    }

    // Add container comment (if any)
    if ( settings.printComments and not container.comment.empty() ) {
        auto parentNode = root.Parent();
        if ( parentNode ) {
            tinyxml2::XMLComment* comment = doc.NewComment(container.comment.c_str());
            doc.InsertAfterChild(parentNode, comment);
        }
    }

    // Add all container arguments
    for ( const auto& attribute : container.attributes.map ) {
        root.SetAttribute(attribute.first.c_str(), attribute.second.c_str());
    }

    // Iterate through all values in this container
    for ( const auto& keyValuePair: container.values ) {

        // Some aliases to make the code easier to read
        const auto& key = keyValuePair.first.c_str();
        const Value& value = keyValuePair.second;

        // Create a new node in the DOM
        tinyxml2::XMLElement* child = nullptr;
        {
            // Nested container
            if ( value.isType<Container*>() ) {
                // Recursion
                const Container* childContainer = value.get<Container*>();
                child = doc.NewElement(key);
                writeEntry(doc, *child, *childContainer);
            }

                // Simple value
            else {
                child = doc.NewElement(key);
                child->SetText(value.toString().data());
            }
        }

        // Add all value arguments
        for ( const auto& attribute : value.attributes.map ) {
            child->SetAttribute(attribute.first.c_str(), attribute.second.c_str());
        }

        GPDS_ASSERT( child );

        // Annotate type if supposed to
        if (settings.annotateTypes and !container.isList()) {
            std::string attributeString = "type";
            if (settings.prefixAnnotations) {
                attributeString = NAMESPACE_PREFIX + attributeString;
            }
            child->SetAttribute(attributeString.c_str(), value.typeString());
        }

        // Add value comment (if any)
        if ( settings.printComments and not value.comment.c_str() ) {
            tinyxml2::XMLComment* comment = doc.NewComment( value.comment.c_str() );
            root.InsertEndChild(comment);
        }

        root.InsertEndChild(child);
    }
}

void ArchiverXml::readEntry(tinyxml2::XMLElement& rootNode, Container& container)
{
    // Handle all nodes children recursively
    for (tinyxml2::XMLElement* node = rootNode.FirstChildElement(); node; node = node->NextSiblingElement()) {
        // Extract the name & value
        gString keyString( node->Name() );

        // Create the Value
        Value value;

        // Container arguments
        for ( const tinyxml2::XMLAttribute* attribute = rootNode.FirstAttribute(); attribute; attribute = attribute->Next() ) {
            container.addAttribute( attribute->Name(), std::string( attribute->Value() ) );
        }

        // It's a text element
        if ( node->GetText() ) {
            value.fromString( std::string(node->GetText()) );

            // Arguments
            {
                // Value arguments
                for (const tinyxml2::XMLAttribute* attribute = node->FirstAttribute(); attribute; attribute = attribute->Next()) {
                    value.addAttribute(attribute->Name(), std::string(attribute->Value()));
                }
            }
        }
        // It's a another container
        else if ( node->FirstChild() ) {
            Container childContainer;

            readEntry(*node, childContainer);
            value.set(std::move(childContainer));
        }

        container.addValue(keyString, value);

    }

}
