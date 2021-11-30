#pragma once

#include <map>
#include "gpds_export.hpp"
#include "value.hpp"
#include "attributes.hpp"
#include "utils.hpp"
#include "types.hpp"

namespace gpds
{
    /**
     * A (de)serializable container.
     *
     * A container holds zero or more values and zero or more attributes.
     */
    class GPDS_EXPORT container
    {
    public:
        std::multimap<gString, value> values;
        class attributes attributes;

        container() = default;
        container(const container& other) = default;
        container(container&& other) noexcept = default;
        virtual ~container() = default;

        container& operator=(const container& rhs) = default;
        container& operator=(container&& rhs) noexcept = default;

        container& operator+=(const container& rhs)
        {
            values.insert(rhs.values.cbegin(), rhs.values.cend());
            attributes += rhs.attributes;
            return *this;
        }

        /**
         * Checks whether the container is empty.
         *
         * A container is considered empty if it holds values and no attributes.
         *
         * @return Whether the container is empty.
         *
         * @sa has_values()
         * @sa has_attributes()
         */
        [[nodiscard]]
        bool empty() const noexcept
        {
            return !has_values() && !has_attributes();
        }

        /**
         * Checks whether the container holds any values.
         *
         * @return Whether the container holds any values.
         *
         * @sa empty()
         * @sa has_attributes()
         */
        [[nodiscard]]
        bool has_values() const noexcept
        {
            return !values.empty();
        }

        /**
         * Checks whether the container holds one or more values of a specific key.
         *
         * @param key The key.
         * @return Whether at least one value matching the key is present.
         */
        [[nodiscard]]
        bool has_value(const std::string& key) const
        {
            return values.find(key) != std::cend(values);
        }

        /**
         * Checks whether the container holds any attributes.
         *
         * @return Whether the container holds any attributes.
         *
         * @sa empty()
         * @sa has_values()
         */
        [[nodiscard]]
        bool has_attributes() const noexcept
        {
            return !attributes.empty();
        }

        /**
         * Checks whether the container holds an attribute of a specific key.
         *
         * @param key The key.
         * @return Whether an attribute matching the key exists.
         */
        [[nodiscard]]
        bool has_attribute(const std::string& key) const
        {
            return attributes.map.find(key) != std::cend(attributes.map);
        }

        template<class T>
        value& add_value(const gString& key, T&& value)
        {
            auto it = values.emplace(std::make_pair(key, std::forward<T>(value)));

            return it->second;
        }

        template<class T>
        [[nodiscard]]
        std::optional<T> get_value(const gString& key) const
        {
            auto it = values.find(key);

            if (it != values.end() && !it->second.is_empty()) {
                const value& value = it->second;

                // T == gpds::value ?
                if constexpr (std::is_same_v<T, gpds::value>)
                    return value;

                else {
                    if (!value.is_type<T>())
                        return std::nullopt;
                    return value.get<T>();
                }
            }

            return std::nullopt;
        }

        template<class T>
        [[nodiscard]]
        std::vector<T> get_values(const gString& key) const
        {
            const auto& range = values.equal_range(key);
            std::vector<T> values;
            for (auto it = range.first; it != range.second; it++) {

                // T == gpds::value ?
                if constexpr (std::is_same_v<T, gpds::value>)
                    values.push_back(it->second);

                else {
                    if (it->second.is_empty() || !it->second.is_type<T>())
                        continue;

                    values.push_back(it->second.get<T>());
                }
            }

            return values;
        }

        template<typename T>
        container& add_attribute(const gString& key, const T& value)
        {
            attributes.add(key, value);

            return *this;
        }

        template<typename T>
        [[nodiscard]]
        std::optional<T> get_attribute(gString&& key) const
        {
            return attributes.get<T>(std::forward<gString>(key));
        }

        template<typename T>
        [[nodiscard]]
        std::optional<T> get_value_attribute(const gString& valueKey, const gString& attributeKey) const
        {
            auto it = values.find(valueKey);

            if (it != values.end() && !it->second.is_empty()) {
                return it->second.get_attribute<T>(attributeKey);
            }

            return std::nullopt;
        }

        [[nodiscard]] bool is_list() const;
    };

}

