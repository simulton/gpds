#pragma once

#include <type_traits>
#include <variant>
#include <string>
#include <vector>
#include <map>
#include <optional>
#include "gpds_export.hpp"
#include "attributes.hpp"
#include "utils.hpp"
#include "types.hpp"

namespace gpds
{
    class container;

    template<class ...Ts> struct overload : Ts... { using Ts::operator()...; };
    template<class ...Ts> overload(Ts...) -> overload<Ts...>;

    template<typename T, typename ...Ts>
    [[nodiscard]]
    static constexpr bool contains()
    {
        return std::disjunction_v<std::is_same<T, Ts> ...>;
    }

    template<typename T>
    [[nodiscard]]
    static constexpr bool is_valid_type()
    {
        return contains<T, gBool, gInt, gReal, gString, gContainer>;
    }

    class GPDS_EXPORT value
    {
    public:
        class attributes attributes;
        gString comment;

        value() = default;
        value(const value& other);
        value(value&& other);
        virtual ~value() noexcept;

        template<class T,
                typename std::enable_if<not std::is_class<T>::value, T>::type* = nullptr>
        value(const T& value)
        {
            set<T>(value);
        }

        template<class T,
                typename std::enable_if<std::is_class<T>::value, T>::type* = nullptr>
        value(T&& value)
        {
            set<T>(std::move(value));
        }

        template<typename T>
        [[nodiscard]]
        constexpr bool is_type() const noexcept
        {
            return std::holds_alternative<T>(m_value);
        }

        [[nodiscard]] constexpr bool is_empty() const
        {
            return m_value.valueless_by_exception();
        }

        [[nodiscard]] constexpr const char* type_string() const
        {
            if (std::holds_alternative<container*>(m_value)) {
                return "nested";
            }

            return std::visit(overload{
                    [](const gBool&)    { return "bool"; },
                    [](const gInt&)     { return "int"; },
                    [](const gReal&)    { return "double"; },
                    [](const gString&)  { return "string"; }
            }, m_value);

            return "n/a";
        }

        void from_string(std::string&& string);
        [[nodiscard]] std::string to_string() const;

        template<typename T>
        void set(const T& value)
        {
            m_value = value;
        }

        template<typename T>
        void set(T&& value)
        {
            m_value = std::move(value);
        }

        template<typename T = std::string>
        void set(const char* string)
        {
            m_value = std::string(string);
        }

        template<typename T = container&>
        void set(const container& container)
        {
            free_container_memory();
            allocate_container_memory(container);
        }

        template<typename T = container&&>
        void set(container&& container)
        {
            free_container_memory();
            allocate_container_memory(std::move(container));
        }

        template<typename T>
        [[nodiscard]]
        constexpr T get() const
        {
            return std::get<T>(m_value);
        }

        template<typename T>
        value& add_attribute(gString&& key, const T& value)
        {
            attributes.add(std::forward<gString>(key), value);

            return *this;
        }

        template<typename T>
        [[nodiscard]]
        std::optional<T> get_attribute(const gString& key) const
        {
            return attributes.get<T>(key);
        }

        value& set_comment(const gString& comment);
        value& set_comment(gString&& comment);

    private:
        std::variant<
            gBool,
            gInt,
            gReal,
            gString,
            gContainer
        > m_value;

        void allocate_container_memory(const container& container);
        void allocate_container_memory(container&& container);
        void free_container_memory();
    };
}
