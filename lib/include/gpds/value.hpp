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

    /**
     * Checks whether a type list contains a specified type.
     *
     * @tparam T The type to check.
     * @tparam Ts The type list.
     * @return Whether the type list contains the specified type.
     */
    template<typename T, typename ...Ts>
    [[nodiscard]]
    static constexpr bool contains()
    {
        return std::disjunction_v<std::is_same<T, Ts> ...>;
    }

    /**
     * Checks whether a specified type is can be held to a value.
     *
     * @tparam T The type.
     * @return Whether the type can be held by a value.
     */
    template<typename T>
    [[nodiscard]]
    static constexpr bool is_valid_type()
    {
        return contains<T, gBool, gInt, gReal, gString, gContainer>;
    }

    /**
     * A (de)serializable value.
     */
    class GPDS_EXPORT value
    {
    public:
        /**
         * Attributes.
         */
        class attributes attributes;

        /**
         * Default constructor.
         */
        value() = default;

        /**
         * Copy constructor.
         *
         * @param other The object to copy-construct from.
         */
        value(const value& other);

        /**
         * Move constructor.
         *
         * @param other The object to move-construct from.
         */
        value(value&& other) noexcept;

        /**
         * Constructor.
         *
         * @tparam T The value type.
         * @param value The value to copy.
         */
        template<
            class T,
            typename std::enable_if<!std::is_class<T>::value, T>::type* = nullptr
        >
        explicit value(const T& value)
        {
            set<T>(value);
        }

        /**
         * Constructor.
         *
         * @tparam T The value type.
         * @param value The value to move.
         */
        template<
            class T,
            typename std::enable_if<std::is_class<T>::value, T>::type* = nullptr
        >
        explicit value(T&& value)
        {
            set<T>(std::forward<T>(value));
        }

        /**
         * Destructor.
         */
        virtual ~value() noexcept;

        /**
         * Copy assignment operator.
         *
         * @param rhs The object to copy-assign from.
         *
         * @return A reference to the lhs object.
         */
        value& operator=(const value& rhs) = default;

        /**
         * Move assignment operator.
         *
         * @param rhs The object to move-assign from.
         *
         * @return A reference to the lhs object.
         */
        value& operator=(value&& rhs) noexcept = default;

        /**
         * Checks whether the value is of a specified type.
         *
         * @tparam T The type.
         * @return Whether the value is of the specified type.
         */
        template<typename T>
        [[nodiscard]]
        constexpr bool is_type() const noexcept
        {
            return std::holds_alternative<T>(m_value);
        }

        /**
         * Checks whether the value is currently empty.
         *
         * @return Whether the value is currently empty.
         */
        [[nodiscard]] constexpr bool is_empty() const
        {
            return m_value.valueless_by_exception();
        }

        /**
         * Get the type as a string.
         *
         * @return The type string.
         */
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

        /**
         * Load the value from a string.
         *
         * @param string The string.
         */
        void from_string(std::string&& string);

        /**
         * Represent the current value as a string.
         *
         * @return The string.
         */
        [[nodiscard]] std::string to_string() const;

        /**
         * Set the value (copy).
         *
         * @tparam T The type of the value.
         * @param value The value.
         */
        template<typename T>
        void set(const T& value)
        {
            m_value = value;
        }

        /**
         * Set the value (move).
         *
         * @tparam T The type of the value.
         * @param value The value.
         */
        template<typename T>
        void set(T&& value)
        {
            m_value = std::forward<T>(value);
        }

        /**
         * Set a C-style string.
         *
         * @tparam T The type.
         * @param string The string.
         */
        template<typename T = std::string>
        void set(const char* string)
        {
            m_value = std::string(string);
        }

        /**
         * Set a container (nested value) (copy).
         *
         * @tparam T The type.
         * @param container The container.
         */
        template<typename T = container&>
        void set(const container& container)
        {
            free_container_memory();
            allocate_container_memory(container);
        }

        /**
         * Set a container (nested value) (move).
         *
         * @tparam T The type.
         * @param container The container.
         */
        template<typename T = container&&>
        void set(container&& container)
        {
            free_container_memory();
            allocate_container_memory(std::move(container));
        }

        /**
         * Get the value.
         *
         * @note This will throw if the contained value does not match T.
         *
         * @tparam T The type.
         * @return The value.
         */
        template<typename T>
        [[nodiscard]]
        constexpr T get() const
        {
            return std::get<T>(m_value);
        }

        /**
         * Add an attribute
         *
         * @tparam T The value type.
         * @param key The key.
         * @param value The value.
         *
         * @return Reference to this.
         */
        template<typename T>
        value& add_attribute(const gString& key, const T& value)
        {
            attributes.add(key, value);

            return *this;
        }

        /**
         * Get an attribute from a specified key (if any).
         *
         * @tparam T The value type.
         * @param key The key.
         * @return The value (if any).
         */
        template<typename T>
        [[nodiscard]]
        std::optional<T> get_attribute(const gString& key) const
        {
            return attributes.get<T>(key);
        }

    private:
        /**
         * Value variant.
         */
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
