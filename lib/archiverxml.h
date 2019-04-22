#pragma once

#include <algorithm>
#include "archiver.h"
#include "3rdparty/rapidxml-1.13/rapidxml_ext.hpp"
#include "3rdparty/rapidxml-1.13/rapidxml_print.hpp"

namespace Gds
{

    class ArchiverXml : public Archiver
    {
    public:
        struct Settings {
            bool annotateLists;
        };

        ArchiverXml()
        {
            settings.annotateLists = true;
        }

        virtual ~ArchiverXml() override = default;

        virtual bool save(std::ostream& stream, const Serialize& object, const std::string& rootName) const override
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
            root->append_attribute(doc.allocate_attribute("version", "1.0"));
            doc.append_node(root);

            // Add the Serialize data
            for (const auto& entry : object.toContainer().entries) {
                writeEntry(doc, *root, entry);
            }

            // Add data to stream
            stream << doc;

            // Free up memory
            doc.clear();

            return true;
        }

        virtual bool load(std::istream& stream, Serialize& object, const std::string& rootName)
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
            Container container;
            readEntry(*rootNode, container);

            // Deserialize the object
            object.fromContainer(container);

            return true;
        }

        Settings settings;

    private:
        void writeEntry(rapidxml::xml_document<>& doc, rapidxml::xml_node<>& root, const std::pair<std::string, Container::Entry>& entry) const
        {
            // Some aliases to make the code easier to read
            const auto& name = doc.allocate_string( entry.first.c_str() );
            const Container::Type& type = entry.second.first;
            const Container::Value& value = entry.second.second;

            // Create a new node in the DOM
            rapidxml::xml_node<>* child = nullptr;
            switch (type) {
                case Container::BoolType:
                    child = doc.allocate_node(rapidxml::node_element, name, doc.allocate_string( std::get<bool>( value ) ? "true" : "false"));
                    break;

                case Container::IntType:
                    child = doc.allocate_node(rapidxml::node_element, name, doc.allocate_string( std::to_string( std::get<int>( value ) ).data() ));
                    break;

                case Container::DoubleType:
                    child = doc.allocate_node(rapidxml::node_element, name, doc.allocate_string( std::to_string( std::get<double>( value ) ).data() ));
                    break;

                case Container::StringType:
                    child = doc.allocate_node(rapidxml::node_element, name, doc.allocate_string( std::get<std::string>( value ).data() ) );
                    break;

                case Container::ContainerType:
                {
                    const Container& container = std::get<Container>( value );
                    child = doc.allocate_node(rapidxml::node_element, name );

                    // Annotate list if supposed to
                    if (settings.annotateLists and container.isList()) {
                        child->append_attribute( doc.allocate_attribute("count", doc.allocate_string( std::to_string( container.entries.size() ).data() ) ) );
                    }

                    // Recursion
                    for (const auto& entry : container.entries) {
                        writeEntry(doc, *child, entry);
                    }

                    break;
                }
            }

            assert(child);
            root.append_node(child);
        }

        void readEntry(rapidxml::xml_node<>& rootNode, Container& container)
        {
            // Handle all nodes children recursively
            for (rapidxml::xml_node<>* node = rootNode.first_node(); node; node = node->next_sibling()) {
                // Extract the name & value
                std::string name( node->name() );
                std::string value( node->value() );

                // It's a text element
                if (!value.empty()) {

                    // Is it a boolean 'true' value?
                    if ( value == "true" ) {
                        container.addEntry( name , Container::BoolType, true );
                        continue;
                    }

                    // Is it a boolean 'false' value?
                    if ( value == "false" ) {
                        container.addEntry( name , Container::BoolType, false );
                        continue;
                    }

                    // Is it an integer?
                    {
                        // Ensure that this is an integer
                        bool isInteger = true;
                        for (std::string::const_iterator it = value.cbegin(); it != value.cend(); ++it) {
                            // Make sure that this is a digit
                            if ( !std::isdigit( static_cast<int>( *it ) ) ) {
                                isInteger = false;
                            }

                            // Check for minus sign
                            if (it == value.cbegin() and !isInteger and *it == '-') {
                                isInteger = true;
                            }

                            if (!isInteger) {
                                break;
                            }
                        }

                        if (isInteger) {
                            try {
                                int i = std::stoi( value );
                                container.addEntry( name, Container::IntType, i);
                                continue;
                            } catch (const std::invalid_argument& e) {
                                (void)e;
                                // Nothing to do here. Fall through.
                            }
                        }
                    }

                    // Is it a double?
                    {

                        try {
                            double d = std::stod( value );
                            container.addEntry( name, Container::DoubleType, d);
                            continue;
                        } catch (const std::invalid_argument& e) {
                            (void)e;
                            // Nothing to do here. Fall through.
                        }
                    }

                    // Lets assume it's just a string :>
                    {
                        container.addEntry( name , Container::StringType, value );
                        continue;
                    }
                }

                // It's a another container
                else {
                    Container childContainer;
                    readEntry(*node, childContainer);
                    container.addEntry( name, Container::ContainerType, childContainer );

                    continue;
                }
            }
        }
    };

}
