#pragma once

#include "concepts.hpp"
#include "value.hpp"
#include "attributes.hpp"
#include "utils.hpp"

#include <map>
#include <optional>

namespace gpds
{
    /**
     * A (de)serializable container.
     *
     * A container holds zero or more values and zero or more attributes.
     */
    class container
    {
    public:
        std::multimap<std::string, value> values;
        class attributes attributes;

        container() = default;
        container(const container& other) = default;
        container(container&& other) noexcept = default;
        virtual ~container() = default;

        container& operator=(const container& rhs) = default;
        container& operator=(container&& rhs) noexcept = default;

        container&
        operator+=(const container& rhs)
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
        bool
        empty() const noexcept
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
        bool
        has_values() const noexcept
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
        bool
        has_value(const std::string& key) const
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
        bool
        has_attributes() const noexcept
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
        bool
        has_attribute(const std::string& key) const
        {
            return attributes.map.find(key) != std::cend(attributes.map);
        }

        template<class T>
        value&
        add_value(const std::string& key, T&& value)
        {
            auto it = values.emplace(std::make_pair(key, std::forward<T>(value)));

            return it->second;
        }

        template<Serializable Object>
        value&
        add_value(const Object& obj)
        {
            return add_value(Object::gpds_name, obj.to_container());
        }

        template<SerializableRange Objects>
        void
        add_values(const Objects& objs)
        {
            for (const auto& obj: objs)
                add_value(obj);
        }

        template<class T>
        [[nodiscard]]
        std::optional<T>
        get_value(const std::string& key) const
        {
            auto it = values.find(key);

            if (it != values.end() && !it->second.is_empty()) {
                const value& value = it->second;

                // T == gpds::value ?
                if constexpr (std::is_same_v<T, gpds::value>)
                    return value;

                else
                    return value.get<T>();
            }

            return std::nullopt;
        }

        template<class T>
        [[nodiscard]]
        T
        get_value(const std::string& key, T&& default_value) const
        {
            return get_value<T>(key).value_or(std::forward<T>(default_value));
        }

        template<Deserializable Object>
        [[nodiscard]]
        std::optional<Object>
        get_value() const
        {
            const gpds::container* c = get_value<gpds::container*>(Object::gpds_name).value_or(nullptr);
            if (!c)
                return std::nullopt;

            Object obj;
            obj.from_container(*c);

            return obj;
        }

        template<class T>
        [[nodiscard]]
        std::vector<T>
        get_values(const std::string& key) const
        {
            const auto& range = values.equal_range(key);

            std::vector<T> values;
            values.reserve(std::distance(range.first, range.second));

            for (auto it = range.first; it != range.second; it++) {

                // T == gpds::value ?
                if constexpr (std::is_same_v<T, gpds::value>)
                    values.push_back(it->second);

                else {
                    if (it->second.is_empty())
                        continue;

                    if (const auto& v_opt = it->second.get<T>(); v_opt)
                        values.push_back(it->second.get<T>().value());
                }
            }

            return values;
        }

        template<Deserializable Object>
        [[nodiscard]]
        std::vector<Object>
        get_values() const
        {
            const auto& values = get_values<gpds::container*>(Object::gpds_name);
            std::vector<Object> ret;
            ret.reserve(std::size(values));

            for (const gpds::container* c : values) {
                if (!c)
                    continue;

                Object obj;
                obj.from_container(*c);

                ret.push_back(std::move(obj));
            }

            return ret;
        }

        template<typename T>
        container&
        add_attribute(const std::string& key, const T& value)
        {
            attributes.add(key, value);

            return *this;
        }

        template<typename T>
        [[nodiscard]]
        std::optional<T>
        get_attribute(std::string&& key) const
        {
            return attributes.get<T>(std::forward<std::string>(key));
        }

        template<typename T>
        [[nodiscard]]
        T
        get_attribute(std::string&& key, T&& default_value) const
        {
            return get_attribute<T>(std::move(key)).value_or(std::forward<T>(default_value));
        }

        template<typename T>
        [[nodiscard]]
        std::optional<T>
        get_value_attribute(const std::string& valueKey, const std::string& attributeKey) const
        {
            auto it = values.find(valueKey);

            if (it != values.end() && !it->second.is_empty()) {
                return it->second.get_attribute<T>(attributeKey);
            }

            return std::nullopt;
        }

        [[nodiscard]]
        bool
        is_list() const
        {
            // We need at least two elements
            if (values.size() < 2)
                return false;

            // Ensure that all elements are the same
            std::string name;
            for (auto it = values.cbegin(); it != values.cend(); ++it) {
                // Store the name so we can compare them
                if (it == values.cbegin())
                    name = it->first;

                // Must not be a container
                if (it->second.is_type<container*>())
                    return false;

                // All elements need to share the same name
                if (it->first != name)
                    return false;
            }

            return true;
        }

    };

}

