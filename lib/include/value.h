#pragma once

#include <type_traits>
#include <variant>
#include <string>
#include <vector>
#include <map>
#include <optional>
#include "gpds_export.h"
#include "attributes.h"
#include "utils.h"
#include "types.h"

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
        return contains< T, gBool, gInt, gReal, gString, gContainer >;
    }

    class GPDS_EXPORT Value
    {
    public:
        Attributes attributes;
        gString comment;

        Value( ) = default;
        Value( const Value& other );
        Value( Value&& other );
        virtual ~Value() noexcept;

        template<class T,
            typename std::enable_if< not std::is_class<T>::value, T >::type* = nullptr>
        Value( const T& value)
        {
            set<T>(value);
        }

        template <class T,
                typename std::enable_if< std::is_class<T>::value, T >::type* = nullptr>
        Value( T&& value )
        {
            set<T>( std::move( value ) );
        }

        template<typename T>
        constexpr bool isType() const noexcept
        {
            return std::holds_alternative<T>( _value );
        }

        constexpr bool isEmpty() const
        {
            return _value.valueless_by_exception();
        }

        constexpr const char* typeString() const
        {
            if ( std::holds_alternative<Container*>( _value ) ) {
                return "nested";
            }

            return std::visit(overload{
                    [](const gBool&)    { return "bool"; },
                    [](const gInt&)     { return "int"; },
                    [](const gReal&)    { return "double"; },
                    [](const gString&)  { return "string"; }
            }, _value);

            return "n/a";
        }

        void fromString(std::string&& string);
        std::string toString() const;

        template<typename T>
        void set(const T& value)
        {
            _value = value;
        }

        template<typename T>
        void set(T&& value)
        {
            _value = std::move( value );
        }

        template<typename T = std::string>
        void set(const char* string)
        {
            _value = std::string(string);
        }

        template<typename T = Container&>
        void set(const Container& container)
        {
            freeContainerMemory();
            allocateContainerMemory(container);
        }

        template<typename T = Container&&>
        void set(Container&& container)
        {
            freeContainerMemory();
            allocateContainerMemory( std::move( container ) );
        }

        template<typename T>
        constexpr T get() const
        {
            return std::get<T>( _value );
        }

        template<typename T>
        Value& addAttribute(gString&& key, const T& value)
        {
            attributes.add( std::forward< gString >( key ), value );

            return *this;
        }

        template<typename T>
        std::optional<T> getAttribute(const gString& key) const
        {
            return attributes.get<T>( key );
        }

        Value& setComment(const gString& comment);
        Value& setComment(gString&& comment);

    private:
        std::variant<
            gBool,
            gInt,
            gReal,
            gString,
            gContainer
        > _value;

        void allocateContainerMemory(const Container& container);
        void allocateContainerMemory(Container&& container);
        void freeContainerMemory();
    };
}
