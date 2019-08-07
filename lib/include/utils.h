#pragma once

#include <optional>
#include "types.h"

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

    template<class T>
    struct is_c_str
            : std::integral_constant<
                    bool,
                    std::is_same<char const *, typename std::decay<T>::type>::value ||
                    std::is_same<char *, typename std::decay<T>::type>::value
            > {};

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
            std::string str = std::to_string( value );
            // Remove trailing zeros
            str.erase( str.find_last_not_of('0') + 1, std::string::npos );
            return str;
        }

        else if constexpr ( std::is_same<T, gString>::value ) {
            return value;
        }

        else if constexpr ( is_c_str<T>::value ) {
            return std::string( value );
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
