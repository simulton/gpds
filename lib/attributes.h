#pragma once

#include <map>
#include <string>
#include <optional>

namespace Gpds
{

    class Attributes
    {
    public:
        std::map< std::string, std::string> map;

        void add(std::string&& key, std::string&& value)
        {
            map.emplace( std::forward< std::string >( key ), std::forward< std::string >( value ) );
        }

        std::optional< std::string > get(const std::string& key) const
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
