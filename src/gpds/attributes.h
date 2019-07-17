#pragma once

#include <map>
#include <string>
#include <optional>
#include <gpds/types.h>

namespace Gpds
{

    class Attributes
    {
    public:
        std::map< gString, gString> map;

        void add(gString&& key, gString&& value)
        {
            map.emplace( std::forward< gString >( key ), std::forward< gString >( value ) );
        }

        std::optional< gString > get(const gString& key) const
        {
            for ( auto it = map.cbegin(); it != map.cend(); ++it ) {
                if ( it->first == key ) {
                    return it->second;
                }
            }

            return std::nullopt;
        }

    };

}
