#pragma once

#include <optional>
#include <gpds/types.h>

namespace Gpds
{
    #define GPDS_ASSERT( x ) \
        if ( !(x) ) {  \
            printf("GPDS Assertion!\n"); \
            printf("  Function : %s\n", __PRETTY_FUNCTION__); \
            printf("  File     : %s\n", __FILE__); \
            printf("  Line     : %d\n", __LINE__); \
            abort();  \
        }

    template<typename T>
    static std::string valueToString( const T& value )
    {
        if constexpr ( std::is_same<T, gBool>::value ) {
            return value ? "true" : "false";
        }

        else if constexpr ( std::is_same<T, gInt>::value ) {
            return std::to_string( value );
        }

        else if constexpr ( std::is_same<T, gReal>::value ) {
            return std::to_string( value );
        }

        else if constexpr ( std::is_same<T, gString>::value ) {
            return value;
        }

        return { };
    }

    template<typename T>
    static std::optional<T> stringToValue( const std::string& string )
    {
        if constexpr ( std::is_same<T, gBool>::value ) {
            return ( string.compare( "true" ) == 0 );
        }

        else if constexpr ( std::is_same<T, gInt>::value ) {
            return std::stoi( string );
        }

        else if constexpr ( std::is_same<T, gReal>::value ) {
            return std::stod( string );
        }

        else if constexpr ( std::is_same<T, gString>::value ) {
            return string;
        }

        return std::nullopt;
    }
}
