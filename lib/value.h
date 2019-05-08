#pragma once

#include <cassert>
#include <type_traits>
#include <variant>
#include <string>
#include <vector>
#include <map>

namespace Gpds
{
    class Container;

    template<class ...Ts> struct overload : Ts... { using Ts::operator()...; };
    template<class ...Ts> overload(Ts...) -> overload<Ts...>;

    template<typename T, typename ...Ts>
    static constexpr bool contains() {
        return std::disjunction_v< std::is_same< T, Ts > ... >;
    }

    template<typename T>
    static constexpr bool isValidType() {
        return contains< T, bool, int, double, std::string, Container* >;
    }

    class Value
    {
    public:
        using myVariant = std::variant<bool, int, double, std::string, Container*>;

        std::map< std::string, std::string > attributes;
        std::string comment;

        Value( ) = default;
        Value( const Value& other ) = default;
        Value( Value&& other ) = default;

        template<class T>
        Value( const T& value, const std::string& comment = std::string() ) :
            comment( comment )
        {
            set<T>(value);
        }

        virtual ~Value() noexcept
        {
            // Ensure that we won't throw
            assert( not _value.valueless_by_exception() );

            // Containers need to be cleaned up
            if ( std::holds_alternative<Container*>( _value ) ) {
#warning ToDO: Free up memory
                //std::cout << "foo = " << std::get<Container*>( _value ) << std::endl;
                //delete std::get<Container*>( _value );
            }
        }

        template<typename T>
        constexpr bool isType() const noexcept
        {
            return std::holds_alternative<T>( _value );
        }

        constexpr const char* typeString() const
        {
            if ( std::holds_alternative<Container*>( _value ) ) {
                return "nested";
            }

            return std::visit(overload{
                    [](const bool&)         { return "bool"; },
                    [](const int&)          { return "int"; },
                    [](const double&)       { return "double"; },
                    [](const std::string&)  { return "string"; }
            }, _value);
        }

        template<typename T>
        void set(const T& value)
        {
            _value = value;
        }

        // Implementation is located in container.h to prevent
        // circular dependency
        template<typename T = Container&>
        void set(const Container& container);

        template<typename T>
        constexpr T get() const
        {
            return std::get<T>( _value );
        }

        void addAttribute(const std::string& key, const std::string& value)
        {
            attributes.emplace( key, value );
        }

        std::string getAttribute(const std::string& key) const
        {
            for ( auto it = attributes.cbegin(); it != attributes.cend(); ++it ) {
                if ( it->first == key ) {
                    return it->second;
                }
            }

            return std::string();
        }

    private:
        myVariant _value;
    };
}
