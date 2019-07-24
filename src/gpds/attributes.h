#pragma once

#include <map>
#include <string>
#include <optional>
#include <gpds/types.h>
#include <gpds/utils.h>

namespace Gpds
{

    class Attributes
    {
    public:
        std::map< gString, gString> map;

        template<typename T>
        void add(gString&& key, T&& value)
        {
            map.emplace( std::forward< gString >( key ), std::forward< gString >( valueToString( value ) ) );
        }

        template<typename T>
        std::optional<T> get(const gString& key) const
        {
            // Get string
            std::string valueString;
            for ( auto it = map.cbegin(); it != map.cend(); ++it ) {
                if ( it->first == key ) {
                    valueString = it->second;
                    break;
                }
            }
            if ( valueString.empty() ) {
                return std::nullopt;
            }

            return stringToValue<T>( valueString );
        }

    };

}
