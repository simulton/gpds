#pragma once

#include <map>
#include <string>
#include <optional>

#include "gpds_export.hpp"
#include "utils.hpp"

namespace gpds
{

    /**
     * A class representing attributes.
     */
    class GPDS_EXPORT attributes
    {
    public:
        /**
         * The map holding our key-value pairs.
         */
        std::map<std::string, std::string> map;

        /**
         * Default constructor.
         */
        attributes() = default;

        /**
         * Copy constructor.
         * @param other The other object to copy from.
         */
        attributes(const attributes& other) = default;

        /**
         * Move constructor.
         * @param other The other object to move from.
         */
        attributes(attributes&& other) noexcept = default;

        /**
         * Destructor.
         */
        virtual ~attributes() = default;

        /**
         * Copy assignment operator.
         *
         * @param rhs The right-hand-side object to copy-assign from.
         * @return A reference to this object.
         */
        attributes& operator=(const attributes& rhs) = default;

        /**
         * Move assignment operator.
         *
         * @param rhs The right-hand-side object to move-assign from.
         * @return A reference to this object.
         */
        attributes& operator=(attributes&& rhs) noexcept = default;

        /**
         * Merge two attribute sets together.
         *
         * @param other The right-hand-side object to merge into this one.
         * @return A reference to this object.
         */
        attributes& operator+=(const attributes& other)
        {
            map.insert(other.map.cbegin(), other.map.cend());
            return *this;
        }

        /**
         * Checks whether there are any attributes.
         *
         * @return Whether there are any attributes.
         */
        [[nodiscard]]
        bool empty() const noexcept
        {
            return map.empty();
        }

        /**
         * Add an attribute.
         *
         * @tparam T The attribute value type.
         * @param key The attribute key.
         * @param value The attribute value.
         */
        template<typename T>
        void add(const std::string& key, const T& value)
        {
            map.emplace(key, value_to_string(value));
        }

        /**
         * Get an attribute.
         *
         * @tparam T The attribute value type.
         * @param key The attribute key.
         * @return The corresponding attribute value (if any).
         */
        template<typename T>
        [[nodiscard]]
        std::optional<T> get(const std::string& key) const
        {
            const auto& it = map.find(key);
            if (it != map.cend())
                return string_to_value<T>(it->second);
            return std::nullopt;
        }

    };

}
