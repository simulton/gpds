#include <gpds/archiverxml.h>

#include <algorithm>
#include <gpds/utils.h>
#include "rapidxml_ext.hpp"
#include "rapidxml_print.hpp"

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
    rapidxml::xml_document<> doc;

    // Add the XML declaration
    rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
    decl->append_attribute(doc.allocate_attribute("version", "1.0"));
    decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
    doc.append_node(decl);

    // Add the root node
    rapidxml::xml_node<>* root = doc.allocate_node(rapidxml::node_element, rootName.data());
    doc.append_node(root);

    // Add the Serialize data
    writeEntry(doc, *root, container);

    // Add data to stream
    stream << doc;

    // Free up memory
    doc.clear();

    return true;
}

bool ArchiverXml::load(std::istream& stream, Container& container, const std::string& rootName)
{
    // Create the document
    std::string string(std::istreambuf_iterator<char>(stream), {});
    rapidxml::xml_document<> doc;
    doc.parse<0>(string.data());

    // Retrieve the root node
    rapidxml::xml_node<>* rootNode = doc.first_node(rootName.data());
    if (!rootNode) {
        return false;
    }

    // Handle all nodes children recursively
    readEntry(*rootNode, container);

    return true;
}

void ArchiverXml::writeEntry(rapidxml::xml_document<>& doc, rapidxml::xml_node<>& root, const Container& container) const
{
    // Annotate list if supposed to
    if (settings.annotateListCount and container.isList()) {
        std::string attributeString = "count";
        if (settings.prefixAnnotations) {
            attributeString = NAMESPACE_PREFIX + attributeString;
        }

        root.append_attribute(doc.allocate_attribute(doc.allocate_string(attributeString.c_str()), doc.allocate_string(std::to_string(container.values.size()).data())));
    }

    // Add container comment (if any)
    if ( settings.printComments and not container.comment.empty() ) {
        auto parentNode = root.parent();
        if ( parentNode ) {
            rapidxml::xml_node<>* commentNode = doc.allocate_node( rapidxml::node_comment, nullptr, container.comment.c_str() );
            parentNode->prepend_node( commentNode );
        }
    }

    // Add all container arguments
    for ( const auto& attribute : container.attributes.map ) {
        root.append_attribute( doc.allocate_attribute( attribute.first.c_str(), attribute.second.c_str() ) );
    }

    // Iterate through all values in this container
    for ( const auto& keyValuePair: container.values ) {

        // Some aliases to make the code easier to read
        const auto& key = doc.allocate_string( keyValuePair.first.c_str() );
        const Value& value = keyValuePair.second;

        // Create a new node in the DOM
        rapidxml::xml_node<>* child = nullptr;
        {
            // Nested container
            if ( value.isType<Container*>() ) {
                // Recursion
                const Container* childContainer = value.get<Container*>();
                child = doc.allocate_node( rapidxml::node_element, key );
                writeEntry(doc, *child, *childContainer);
            }

                // Simple value
            else {
                child = doc.allocate_node(rapidxml::node_element, key, doc.allocate_string( value.toString().data() ) );
            }
        }

        // Add all value arguments
        for ( const auto& attribute : value.attributes.map ) {
            child->append_attribute( doc.allocate_attribute( attribute.first.c_str(), attribute.second.c_str() ) );
        }

        GPDS_ASSERT( child );

        // Annotate type if supposed to
        if (settings.annotateTypes and !container.isList()) {
            std::string attributeString = "type";
            if (settings.prefixAnnotations) {
                attributeString = NAMESPACE_PREFIX + attributeString;
            }
            child->append_attribute( doc.allocate_attribute( doc.allocate_string( attributeString.c_str() ), value.typeString() ) );
        }

        // Add value comment (if any)
        if ( settings.printComments and not value.comment.c_str() ) {
            rapidxml::xml_node<>* commentNode = doc.allocate_node( rapidxml::node_comment, nullptr, value.comment.c_str() );
            root.append_node( commentNode );
        }

        root.append_node(child);
    }
}

void ArchiverXml::readEntry(rapidxml::xml_node<>& rootNode, Container& container)
{
    // Handle all nodes children recursively
    for (rapidxml::xml_node<>* node = rootNode.first_node(); node; node = node->next_sibling()) {
        // Extract the name & value
        gString keyString( node->name() );
        std::string valueString( node->value() );

        // Create the Value
        Value value;

        // Container arguments
        for ( const rapidxml::xml_attribute<>* attribute = rootNode.first_attribute(); attribute; attribute = attribute->next_attribute() ) {
            container.addAttribute( attribute->name(), attribute->value() );
        }

        // Ensure that it's not an empty element
        if ( node->first_node() ) {

            // It's a text element
            if ( not valueString.empty() ) {
                value.fromString( std::move( valueString ));

                // Arguments
                {
                    // Value arguments
                    for (const rapidxml::xml_attribute<> *attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
                        value.addAttribute(attribute->name(), attribute->value());
                    }
                }
            }

                // It's a another container
            else {
                Container childContainer;

                readEntry(*node, childContainer);
                value.set(std::move(childContainer));
            }

            container.addValue(keyString, value);

        }
    }

}
