#pragma once

#include <algorithm>
#include "archiver.h"
#include "3rdparty/rapidxml-1.13/rapidxml_ext.hpp"
#include "3rdparty/rapidxml-1.13/rapidxml_print.hpp"
#include <iostream>
namespace Gpds
{

    class ArchiverXml : public Archiver
    {
    public:
        // Deal with name hiding
        using Archiver::save;
        using Archiver::load;

        const std::string NAMESPACE_PREFIX = "gpds:";

        struct Settings {
            bool annotateListCount;
            bool annotateTypes;
            bool prefixAnnotations;
        };

        ArchiverXml()
        {
            settings.annotateListCount = true;
            settings.annotateTypes = false;
            settings.prefixAnnotations = true;
        }

        virtual ~ArchiverXml() override = default;

        virtual bool save(std::ostream& stream, const Container& container, const std::string& rootName) const override
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

        virtual bool load(std::istream& stream, Container& container, const std::string& rootName) override
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

        Settings settings;

    private:
        void writeEntry(rapidxml::xml_document<>& doc, rapidxml::xml_node<>& root, const Container& container) const
        {
            // Annotate list if supposed to
            if (settings.annotateListCount and container.isList()) {
                std::string attributeString = "count";
                if (settings.prefixAnnotations) {
                    attributeString = NAMESPACE_PREFIX + attributeString;
                }

                root.append_attribute(doc.allocate_attribute(doc.allocate_string(attributeString.c_str()), doc.allocate_string(std::to_string(container.values.size()).data())));
            }

            // Iterate through all values in this container
            for ( const auto& keyValuePair: container.values ) {

                // Some aliases to make the code easier to read
                const auto& key = doc.allocate_string( keyValuePair.first.c_str() );
                const Value& value = keyValuePair.second;

                // Add all arguments
                for ( const auto& attribute : value.attributes ) {
                    root.append_attribute( doc.allocate_attribute( attribute.first.data(), attribute.second.data() ) );
                }

                // Create a new node in the DOM
                rapidxml::xml_node<>* child = nullptr;
                {
                    if ( value.isType<bool>() ) {
                        child = doc.allocate_node(rapidxml::node_element, key, doc.allocate_string( value.get<bool>() ? "true" : "false"));
                    }
                    else if ( value.isType<int>() ) {
                        child = doc.allocate_node(rapidxml::node_element, key, doc.allocate_string( std::to_string( value.get<int>() ).data() ));
                    }
                    else if ( value.isType<double>() ) {
                        child = doc.allocate_node(rapidxml::node_element, key, doc.allocate_string( std::to_string( value.get<double>() ).data() ));
                    }
                    else if ( value.isType<std::string>() ) {
                        child = doc.allocate_node(rapidxml::node_element, key, doc.allocate_string( value.get<std::string>().data() ) );
                    }
                    else if ( value.isType<Container*>() ) {
                        // Recursion
                        const Container* childContainer = value.get<Container*>();
                        child = doc.allocate_node( rapidxml::node_element, key );
                        writeEntry(doc, *child, *childContainer);
                    }
                    else {
                        // This shouldn't happen
                        assert( false );
                    }
                }

                assert(child);

                // Annotate type if supposed to
                if (settings.annotateTypes and !container.isList()) {
                    std::string attributeString = "type";
                    if (settings.prefixAnnotations) {
                        attributeString = NAMESPACE_PREFIX + attributeString;
                    }
                    child->append_attribute( doc.allocate_attribute( doc.allocate_string( attributeString.c_str() ), value.typeString() ) );
                }

                root.append_node(child);
            }
        }

        void readEntry(rapidxml::xml_node<>& rootNode, Container& container)
        {
            // Handle all nodes children recursively
            for (rapidxml::xml_node<>* node = rootNode.first_node(); node; node = node->next_sibling()) {
                // Extract the name & value
                std::string keyString( node->name() );
                std::string valueString( node->value() );

                // Create the Value
                Value value;

                // Arguments
                for ( const rapidxml::xml_attribute<>* attribute = rootNode.first_attribute(); attribute; attribute = attribute->next_attribute() ) {
                    value.addAttribute( attribute->name(), attribute->value() );
                }

                // It's a text element
                if ( not valueString.empty() ) {
                    // Is it a boolean 'true' value?
                    if ( valueString == "true" ) {
                        value.set( true );
                        goto stringParsed;
                    }

                    // Is it a boolean 'false' value?
                    if ( valueString == "false" ) {
                        value.set( false );
                        goto stringParsed;
                    }

                    // Is it an integer?
                    {
                        // Ensure that this is an integer
                        bool isInteger = true;
                        for (std::string::const_iterator it = valueString.cbegin(); it != valueString.cend(); ++it) {
                            // Make sure that this is a digit
                            if ( not std::isdigit( static_cast<int>( *it ) ) ) {
                                isInteger = false;
                            }

                            // Check for minus sign
                            if ( it == valueString.cbegin() and !isInteger and *it == '-' ) {
                                isInteger = true;
                            }

                            if ( not isInteger ) {
                                break;
                            }
                        }

                        if ( isInteger ) {
                            try {
                                int i = std::stoi( valueString );
                                value.set( i );
                                goto stringParsed;
                            } catch (const std::invalid_argument& e) {
                                (void)e;
                                // Nothing to do here. Fall through.
                            }
                        }
                    }

                    // Is it a double?
                    {

                        try {
                            double d = std::stod( valueString );
                            value.set( d );
                            goto stringParsed;
                        } catch (const std::invalid_argument& e) {
                            (void)e;
                            // Nothing to do here. Fall through.
                        }
                    }

                    // Lets assume it's just a string :>
                    {
                        value.set( valueString );
                        goto stringParsed;
                    }

                    stringParsed:
                    // Nothing to do here
                    int i = 0;
                    (void)i;
                }

                // It's a another container
                else {
                    Container childContainer;

                    readEntry(*node, childContainer);
                    value.set( std::move( childContainer ) );
                }

                container.addValue( keyString, value );
            }
        }
    };

}
