#pragma once

#include <map>
#include <string>
#include <optional>
#include "gpds_export.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace gpds
{

    class GPDS_EXPORT attributes
    {
    public:
        std::map<gString, gString> map;

        attributes() = default;
        attributes(const attributes& other);
        attributes(attributes&& other);
        virtual ~attributes() = default;

        attributes& operator+=(const attributes& other)
        {
            map.insert(other.map.cbegin(), other.map.cend());
            return *this;
        }

        template<typename T>
        void add(const gString& key, const T& value)
        {
            map.emplace(key, value_to_string(value));
        }

        template<typename T>
        [[nodiscard]]
        std::optional<T> get(const gString& key) const
        {
            // Get string
            std::string valueString;
            for (auto it = map.cbegin(); it != map.cend(); ++it) {
                if (it->first == key) {
                    valueString = it->second;
                    break;
                }
            }
            if (valueString.empty()) {
                return std::nullopt;
            }

            return string_to_value<T>(valueString);
        }

    };

}
