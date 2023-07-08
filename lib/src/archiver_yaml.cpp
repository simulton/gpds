#include "archiver_yaml.hpp"
#include "utils.hpp"
#include "../3rdparty/miniyaml/yaml/yaml.hpp"

#include <algorithm>

using namespace gpds;

bool
archiver_yaml::save(std::ostream& stream, const container& container, std::string_view root_name) const
{
    // Create the YAML document node
    Yaml::Node root;
    Yaml::Node node;
    // Handle all nodes children recursively
    write_entry(node, container);
    // Add child node to root
    root[root_name.data()] = node;
    // Serialize the document
    std::string data = "";
    Yaml::Serialize(root, data);
    // Write the document to the stream
    stream << data;

    return true;
}

bool
archiver_yaml::load(std::istream& stream, container& container, std::string_view root_name)
{
    // Create the YAML document node
    Yaml::Node root;
    Yaml::Node node;
    // Read istream into string
    std::string data((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    // Parse the document
    Yaml::Parse(root, data);
    // Retrieve the data to container
    read_entry(root[root_name.data()], container);

    return true;
}

void
archiver_yaml::write_entry(Yaml::Node& root, const container& container) const
{
    // Annotate list if supposed to
    if (settings.annotate_list_count && container.is_list()) {
        std::string attributeString = "count";
        if (settings.prefix_annotations) {
            // Generate attribute string
            attributeString = NAMESPACE_PREFIX + attributeString;
        }
        root[attributeString] = std::to_string(container.values.size());
    }

    // Add all container arguments
    for (const auto& attribute : container.attributes.map) {
        // Add "-" prefix for container attributes
        root["-" + attribute.first] = attribute.second;
    }

    // Iterate through all values in this container
    for (const auto& keyValuePair: container.values) {

        // Some aliases to make the code easier to read
        const auto& key = keyValuePair.first.c_str();
        const value& value = keyValuePair.second;

        // Create a new node in the DOM
        Yaml::Node child;

        bool has_attribute = false;
        // Add all value arguments
        for (const auto& attribute : value.attributes.map) {
            // Add "-" prefix for value attributes
            child["-" + attribute.first] = attribute.second;
            has_attribute = true;
        }

        {
            // Nested container
            if (value.is_type<gpds::container*>()) {
                // Recursion
                const auto child_container = value.get<gpds::container*>().value_or(nullptr);
                if (child_container) {
                    // Write child container
                    write_entry(child, *child_container);
                }
            }
            // Simple value
            else {
                // Create text
                std::string text = value.get<std::string>().value_or("");
                // Trim whitespace
                std::string whitespace = " \n\r\t\f\v";
                size_t start = text.find_first_not_of(whitespace);
                size_t end = text.find_last_not_of(whitespace);
                text = (start == std::string::npos) ? "" : text.substr(start);
                text = (end == std::string::npos) ? "" : text.substr(0, end + 1);
                // Set null if text is empty
                if (text.empty()) {
                    text = "null";
                }
                // Set text value by attribute if there are attributes
                if (has_attribute) {
                    // Set "#text" attribute if there are attributes
                    child["#text"] = text;
                } else {
                    // Set text to value if there are no attributes
                    child = text;
                }
                // Set "#cdata" attribute if it is cdata
                if (value.use_cdata()) {
                    child["#cdata"] = text;
                }
            }
        }

        // Is key existing?
        bool key_exist = false;
        switch (root[key].Type())
        {
        case Yaml::Node::eType::None:
            key_exist = false;
            break;
        case Yaml::Node::eType::SequenceType:
        case Yaml::Node::eType::MapType:
        case Yaml::Node::eType::ScalarType:
            key_exist = true;
            break;
        }

        if (key_exist) {
            // If repeated key, add to sequence
            if (root[key].Type() == Yaml::Node::eType::MapType || Yaml::Node::eType::ScalarType) {
                Yaml::Node first_node = root[key];
                root[key].PushBack() = first_node;
            }
            root[key].PushBack() = child;
        } else {
            // If new key, add to map
            root[key] = child;
        }
    }
}

void
archiver_yaml::read_entry(const Yaml::Node& root, container& container)
{
    // Iterate through all children
    for(auto it = root.Begin(); it != root.End(); it++)
    {
        const std::string& it_key = (*it).first;
        const Yaml::Node& it_node = (*it).second;
        switch (it_node.Type())
        {
        case Yaml::Node::eType::None:
            break;
        case Yaml::Node::eType::SequenceType:
            // Handle all SequenceType children recursively
            for(auto it = it_node.Begin(); it != it_node.End(); it++)
            {
                const Yaml::Node& seq_node = (*it).second;
                gpds::container child_container;
                // Check if this node is a text node
                if (seq_node.Type() == Yaml::Node::eType::ScalarType) {
                    std::string seq_text = seq_node.As<std::string>();
                    // Set empty if text is null
                    if (seq_text == "null") {
                        seq_text = "";
                    }
                    // Set text value
                    container.add_value(it_key, seq_text);
                } else {
                    // Recursion for child container
                    read_entry(seq_node, child_container);
                    if (!it_key.empty()) {
                        container.add_value(it_key, std::move(child_container));
                    }
                }
            }
            break;
        case Yaml::Node::eType::MapType:
            // Handle all MapType children recursively
            {
                gpds::value child_value;
                bool is_text = false;
                // Check if this node is a text node
                for(auto it = it_node.Begin(); it != it_node.End(); it++)
                {
                    const std::string& map_key = (*it).first;
                    const Yaml::Node& map_node = (*it).second;
                    if (map_node.Type() == Yaml::Node::eType::ScalarType) {
                        std::string map_text = map_node.As<std::string>();
                        // Set empty if text is null
                        if (map_text == "null") {
                            map_text = "";
                        }
                        if (map_key == "#text") {
                            // This node is a text node
                            is_text = true;
                            child_value.set(map_text);
                        } else if (map_key == "#cdata") {
                            child_value.set(map_text);
                            child_value.set_use_cdata(true);
                            //container.add_value(it_key, child_value);
                        } else if (map_key[0] == '-') {
                            // It's a value attribute
                            std::string attribute_key = map_key;
                            std::string attribute_value = map_text;
                            child_value.add_attribute(attribute_key.erase(0, 1), attribute_value);
                        } else {
                            // It's a value
                            child_value.set(map_text);
                        }
                    }
                }
                // Add node to container
                if (is_text) {
                    // Add text node to container
                    container.add_value(it_key, std::move(child_value));
                } else {
                    // Handle all MapType children recursively
                    gpds::container child_container;
                    read_entry(it_node, child_container);
                    if (!it_key.empty()) {
                        container.add_value(it_key, std::move(child_container));
                    }
                }
            }
            break;
        case Yaml::Node::eType::ScalarType:
            // Handle ScalarType value
            std::string it_text = (*it).second.As<std::string>();
            std::string attribute_key = it_key;
            std::string attribute_value = it_text;
            if (it_text == "null") {
                it_text = "";
            }
            if (it_key[0] == '-') {
                // It's a container attribute
                container.add_attribute(attribute_key.erase(0, 1), it_text);
            } else {
                // It's a text element
                container.add_value(it_key, it_text);
            }
            break;
        }
    }
}
