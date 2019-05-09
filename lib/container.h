#pragma once

#include <cassert>
#include <map>
#include "value.h"
#include "attributes.h"

namespace Gpds
{
    class Container
    {
    public:
        Attributes attributes;

        Container() = default;
        Container(const Container& other) = default;
        Container( Container&& other ) = default;

        template<class T>
        Value& addValue( const std::string& key, T&& value )
        {
            auto it = values.emplace( std::make_pair(key, std::forward<T>( value )) );

            return it->second;
        }

        template<class T>
        T getValue( const std::string& key, T&& defaultValue = T() ) const
        {
            auto it = values.find( key );

            if ( it != values.end() ) {
                const Value& value = it->second;

                assert( value.isType<T>() );

                return it->second.get<T>();
            }

            return std::move( defaultValue );
        }

        template<class T>
        std::vector<T> getValues(const std::string& key) const
        {
            const auto& range = values.equal_range( key );
            std::vector<T> values( std::distance( range.first, range.second ) );
            for (auto it = range.first; it != range.second; it++) {
                values[ std::distance( range.first, it ) ] = it->second.get<T>();
            }

            return values;
        }

        Container& addAttribute(std::string&& key, std::string&& value)
        {
            attributes.addAttribute( std::forward< std::string >( key ), std::forward< std::string >( value ) );

            return *this;
        }

        Container& addAttribute(std::string&& key, const std::string& value)
        {
            return addAttribute( std::forward< std::string >( key ), std::string( value ) ) ;
        }

        std::optional< std::string > getAttribute(std::string&& key) const
        {
            return attributes.getAttribute( std::forward< std::string >( key ) );
        }

        bool isList() const
        {
            // We need at least two elements
            if (values.size() < 2) {
                return false;
            }

            // Ensure that all elements are the same
            std::string_view name;
            for (auto it = values.cbegin(); it != values.cend(); ++it) {
                // Store the name so we can compare them
                if (it == values.begin()) {
                    name = it->first;
                }

                // Gotta be a container type
                if ( not it->second.isType<Container*>() ) {
                    return false;
                }

                // All elements need to share the same name
                if (it->first != name) {
                    return false;
                }
            }

            return true;
        }

        std::multimap< std::string, Value > values;
    };

    // Not located in value.h to prevent circular dependency
    void Value::freeContainerMemory()
    {
        // Containers need to be cleaned up
        if ( std::holds_alternative<Container*>( _value ) ) {
            delete std::get<Container*>( _value );
            _value = nullptr;
        }
    }

    // Not located in value.h to prevent circular dependency
    void Value::allocateContainerMemory(const Container& container)
    {
        _value = new Container( container );
    }

    // Not located in value.h to prevent circular dependency
    void Value::allocateContainerMemory(Container&& container)
    {
        _value = new Container( std::move( container ) );
    }
}

