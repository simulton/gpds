#pragma once

#ifdef GPDS_SUPPORT_QT
    #include <QString>
#endif
#include <type_traits>
#include <variant>
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <gpds/attributes.h>
#include <gpds/utils.h>
#include <gpds/types.h>

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

    class Value
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

        constexpr bool isEmpty() const;
        constexpr const char* typeString() const;
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

        Value& addAttribute(gString&& key, gString&& value);
        Value& addAttribute(gString&& key, const gString& value);

        template<typename T>
        std::optional<T> getAttribute(gString&& key) const
        {
            return attributes.get<T>( std::forward<gString>( key ) );
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
            #ifdef GPDS_SUPPORT_QT
                ,QString,
            #endif
        > _value;

        // Implementation is located in value.cpp to prevent
        // circular dependency.
        // This keeps this library from being a header-only library. Maybe
        // someone could tell me how to fix this in a way that we can achieve
        // a header-only library again.
        void allocateContainerMemory(const Container& container);
        void allocateContainerMemory(Container&& container);
        void freeContainerMemory();
    };
}
