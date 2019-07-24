#pragma once

#include <map>
#include <gpds/value.h>
#include <gpds/attributes.h>
#include <gpds/utils.h>
#include <gpds/types.h>

namespace Gpds
{
    class Container
    {
    public:
        Attributes attributes;
        gString comment;

        Container() = default;
        Container(const Container& other) = default;
        Container( Container&& other ) = default;

        template<class T>
        Value& addValue( const gString& key, T&& value )
        {
            auto it = values.emplace( std::make_pair(key, std::forward<T>( value )) );

            return it->second;
        }

        template<class T>
        T getValue( const gString& key, T&& defaultValue = T() ) const
        {
            auto it = values.find( key );

            if ( it != values.end() and not it->second.isEmpty() ) {
                const Value& value = it->second;

                if ( not value.isType<T>() ) {
                    return std::move( defaultValue );
                }

                return it->second.get<T>();
            }

            return std::move( defaultValue );
        }

        template<class T>
        std::vector<T> getValues(const gString& key) const
        {
            const auto& range = values.equal_range( key );
            std::vector<T> values( std::distance( range.first, range.second ) );
            for (auto it = range.first; it != range.second; it++) {
                values[ std::distance( range.first, it ) ] = it->second.get<T>();
            }

            return values;
        }

        Container& addAttribute(gString&& key, gString&& value)
        {
            attributes.add( std::forward< gString >( key ), std::forward< gString >( value ) );

            return *this;
        }

        Container& addAttribute(gString&& key, const gString& value)
        {
            return addAttribute( std::forward< gString >( key ), gString( value ) );
        }

        template<typename T>
        std::optional<T> getAttribute(gString&& key) const
        {
            return attributes.get<T>( std::forward< gString >( key ) );
        }

        std::optional<gString> getValueAttribute(const gString& valueKey, const gString& attributeKey) const
        {
            auto it = values.find( valueKey );

            if ( it != values.end() and not it->second.isEmpty() ) {
                return it->second.getAttribute( attributeKey );
            }

            return std::nullopt;
        }

        Container& setComment(const gString& comment)
        {
            this->comment = comment;

            return *this;
        }

        Container& setComment(gString&& comment)
        {
            this->comment = std::move( comment );

            return *this;
        }

        bool isList() const
        {
            // We need at least two elements
            if (values.size() < 2) {
                return false;
            }

            // Ensure that all elements are the same
            gString name;
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

        std::multimap< gString, Value > values;
    };

}

