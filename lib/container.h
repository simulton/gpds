#pragma once

#include <cassert>
#include <type_traits>
#include <variant>
#include <list>
#include <map>
#include <vector>

namespace Gds
{

    class Container
    {
    public:
        enum ValueType {
            BoolType,
            IntType,
            DoubleType,
            StringType,
            ContainerType
        };

        using Value     = std::variant<bool, int, double, std::string, Container>;
        using Entry     = std::pair< ValueType, Value >;
        using Entries   = std::multimap< std::string, Entry >;
        using Arguments = std::map< std::string, std::string >;

        // Helper function to retrieve the index of the type T in the the VariantType
        template<class VariantType, class T, std::size_t index = 0>
        static constexpr std::size_t variant_index()
        {
            if constexpr (index == std::variant_size_v<VariantType>) {
                return index;
            }

            else if constexpr (std::is_same_v<std::variant_alternative_t<index, VariantType>, T>) {
                return index;
            }

            else {
                return variant_index<VariantType, T, index + 1>();
            }
        }

        static constexpr const char* typeString(ValueType type)
        {
            switch (type) {
                case BoolType:      return "bool";
                case IntType:       return "int";
                case DoubleType:    return "double";
                case StringType:    return "string";
                case ContainerType: return "container";
            }
        }

        Container(const std::string& name = std::string()) :
            name(name)
        {
        }

        void addEntry(const std::string& name, ValueType type, const Value& value)
        {
            entries.emplace(name, std::make_pair(type, value));
        }

        void addEntry(const std::string& name, bool value)
        {
            addEntry(name, BoolType, value);
        }

        void addEntry(const std::string& name, int value)
        {
            addEntry(name, IntType, value);
        }

        void addEntry(const std::string& name, double value)
        {
            addEntry(name, DoubleType, value);
        }

        void addEntry(const std::string& name, const std::string& value)
        {
            addEntry(name, StringType, value);
        }

        void addEntry(const std::string& name, const Container& value)
        {
            this->name = name;
            addEntry(name, ContainerType, value);
        }

        template<class T>
        T getEntry(const std::string& name) const
        {
            // Find the first entry
            auto it = entries.find( name );
            if (it == entries.end()) {
                return T();
            }

            // Extract value & type info
            const Value& value = it->second.second;
            const std::size_t& typeIndexVariant = value.index();
            std::size_t typeIndexTemplate = static_cast<std::size_t>( variant_index<Value, T>() );

            // Ensure that T and the variant type are the same
            if ( typeIndexVariant != typeIndexTemplate ) {
                return T();
            }

            return std::get<T>( value );
        }

        template<class T>
        std::vector<T> getEntries(const std::string& name) const
        {
            const auto& range = entries.equal_range( name );
            std::vector<T> entries( std::distance( range.first, range.second ) );
            for (auto it = range.first; it != range.second; it++) {
                entries[ std::distance( range.first, it ) ] = std::get<T>(it->second.second);
            }

            return entries;
        }

        void addArgument(const std::string& key, const std::string& value)
        {
            arguments.emplace( key, value );
        }

        std::string getAttribute(const std::string& key) const
        {
            for ( auto it = arguments.cbegin(); it != arguments.cend(); ++it ) {
                if ( it->first == key ) {
                    return it->second;
                }
            }

            return std::string();
        }

        bool isList() const
        {
            // We need at least two elements
            if (entries.size() < 2) {
                return false;
            }

            // Ensure that all elements are the same
            std::string_view name;
            for (auto it = entries.cbegin(); it != entries.cend(); ++it) {
                // Store the name so we can compare them
                if (it == entries.begin()) {
                    name = it->first;
                }

                // Gotta be a container type
                if (it->second.first != ContainerType) {
                    return false;
                }

                // All elements need to share the same name
                if (it->first != name) {
                    return false;
                }
            }

            return true;
        }

        std::string name;
        Arguments arguments;
        Entries entries;
    };

}
