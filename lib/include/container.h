#pragma once

#include <map>
#include "gpds_export.h"
#include "value.h"
#include "attributes.h"
#include "utils.h"
#include "types.h"

namespace Gpds
{
    class GPDS_EXPORT Container
    {
    public:
        std::multimap< gString, Value > values;
        Attributes attributes;
        gString comment;

        Container() = default;
        Container(const Container& other) = default;
        Container( Container&& other ) = default;

        Container& operator+=(const Container& rhs) {
            values.insert(rhs.values.cbegin(), rhs.values.cend());
            attributes += rhs.attributes;
            return *this;
        }

        template<class T>
        Value& addValue( const gString& key, T&& value )
        {
            auto it = values.emplace( std::make_pair(key, std::forward<T>( value )) );

            return it->second;
        }

        template<class T>
        [[nodiscard]]
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
        [[nodiscard]]
        std::vector<T> getValues(const gString& key) const
        {
            const auto& range = values.equal_range( key );
            std::vector<T> values( std::distance( range.first, range.second ) );
            for (auto it = range.first; it != range.second; it++) {
                values[ std::distance( range.first, it ) ] = it->second.get<T>();
            }

            return values;
        }

        template<typename T>
        Container& addAttribute(gString&& key, const T& value)
        {
            attributes.add( std::forward< gString >( key ), value );

            return *this;
        }

        template<typename T>
        [[nodiscard]]
        std::optional<T> getAttribute(gString&& key) const
        {
            return attributes.get<T>( std::forward< gString >( key ) );
        }

        template<typename T>
        [[nodiscard]]
        std::optional<T> getValueAttribute(const gString& valueKey, const gString& attributeKey) const
        {
            auto it = values.find( valueKey );

            if ( it != values.end() and not it->second.isEmpty() ) {
                return it->second.getAttribute<T>( attributeKey );
            }

            return std::nullopt;
        }

        Container& setComment(const gString& comment);
        Container& setComment(gString&& comment);
        [[nodiscard]] bool isList() const;
    };

}

