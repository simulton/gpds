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

        void addAttribute(std::string&& key, std::string&& value)
        {
            map.emplace( std::move( key ), std::move( value ) );
        }

        std::optional< std::string > getAttribute(const std::string& key) const
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
