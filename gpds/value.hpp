#pragma once

#include "attributes.hpp"
#include "utils.hpp"

#include <type_traits>
#include <variant>
#include <string>
#include <vector>
#include <map>
#include <optional>

namespace gpds
{
    class container;

    /**
     * A (de)serializable value.
     *
     * A value can have zero or more attributes.
     */
    class value
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
        template<typename T>
        requires (!std::is_class_v<T>)
        explicit
        value(const T& value)
        {
            set<T>(value);
        }

        /**
         * Constructor.
         *
         * @tparam T The value type.
         * @param value The value to move.
         */
        template<typename T>
        requires (std::is_class_v<T>)
        explicit
        value(T&& value)
        {
            set<T>(std::forward<T>(value));
        }

        /**
         * Destructor.
         */
        virtual
        ~value() noexcept;

        /**
         * Copy assignment operator.
         *
         * @param rhs The object to copy-assign from.
         *
         * @return A reference to the lhs object.
         */
        value&
        operator=(const value& rhs) = default;

        /**
         * Move assignment operator.
         *
         * @param rhs The object to move-assign from.
         *
         * @return A reference to the lhs object.
         */
        value&
        operator=(value&& rhs) noexcept = default;

        /**
         * Checks whether the value is of a specified type.
         *
         * @tparam T The type.
         * @return Whether the value is of the specified type.
         */
        template<typename T>
        [[nodiscard]]
        constexpr
        bool
        is_type() const noexcept
        {
            return std::holds_alternative<T>(m_value);
        }

        /**
         * Checks whether the value is currently empty.
         *
         * @return Whether the value is currently empty.
         */
        [[nodiscard]]
        constexpr
        bool
        is_empty() const noexcept
        {
            return m_value.valueless_by_exception();
        }

        /**
         * Set the value (copy).
         *
         * @tparam T The type of the value.
         * @param value The value.
         */
        template<typename T>
        void
        set(const T& value)
        {
            // Container
            if constexpr (std::is_same_v<gpds::container*, T>) {
                free_container_memory();
                allocate_container_memory(*value);
            }

            // Convert
            else
                m_value = value_to_string(value);
        }

        /**
         * Set a container (nested value) (copy).
         *
         * @tparam T The type.
         * @param container The container.
         */
        template<typename T = container&>
        void
        set(const container& container)
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
        void
        set(container&& container)
        {
            free_container_memory();
            allocate_container_memory(std::move(container));
        }

        /**
         * Get the value.
         *
         * @tparam T The type.
         * @return The value.
         */
        template<typename T>
        [[nodiscard]]
        constexpr
        std::optional<T>
        get() const
        {
            if constexpr (std::is_same_v<container*, T> || std::is_same_v<const container*, T>) {
                if (std::holds_alternative<container*>(m_value))
                    return std::get<container*>(m_value);
            }

            else {
                if (std::holds_alternative<std::string>(m_value))
                    return string_to_value<T>(std::get<std::string>(m_value));
            }

            return std::nullopt;
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
        value&
        add_attribute(const std::string& key, const T& value)
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
        std::optional<T>
        get_attribute(const std::string& key) const
        {
            return attributes.get<T>(key);
        }

        /**
         * Controls whether to use CDATA for serialization.
         *
         * @param enabled Whether to use CDATA for serialization.
         */
        void
        set_use_cdata(const bool enabled) noexcept
        {
            m_use_cdata = enabled;
        }

        [[nodiscard]]
        constexpr
        bool
        use_cdata() const noexcept
        {
            return m_use_cdata;
        }

    private:
        /**
         * Value variant.
         */
        std::variant<
            std::string,
            container*
        > m_value;

        /**
         * Whether to use CDATA for serialization.
         */
        bool m_use_cdata = false;

        void allocate_container_memory(const container& container);
        void allocate_container_memory(container&& container);
        void free_container_memory();
    };
}
