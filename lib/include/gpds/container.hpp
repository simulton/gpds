#pragma once

#include <map>
#include "gpds_export.hpp"
#include "value.hpp"
#include "attributes.hpp"
#include "utils.hpp"
#include "types.hpp"

namespace gpds
{
    class GPDS_EXPORT container
    {
    public:
        std::multimap<gString, value> values;
        class attributes attributes;
        gString comment;

        container() = default;
        container(const container& other) = default;
        container(container&& other) = default;

        container& operator+=(const container& rhs)
        {
            values.insert(rhs.values.cbegin(), rhs.values.cend());
            attributes += rhs.attributes;
            return *this;
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

            if (it != values.end() and not it->second.is_empty()) {
                const value& value = it->second;

                if (not value.is_type<T>()) {
                    return std::nullopt;
                }

                return it->second.get<T>();
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
                if (it->second.is_empty() or not it->second.is_type<T>()) {
                    continue;
                }
                values.push_back(it->second.get<T>());
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

            if (it != values.end() and not it->second.is_empty()) {
                return it->second.get_attribute<T>(attributeKey);
            }

            return std::nullopt;
        }

        container& set_comment(const gString& comment);
        container& set_comment(gString&& comment);
        [[nodiscard]] bool is_list() const;
    };

}

