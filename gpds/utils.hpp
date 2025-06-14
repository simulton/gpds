#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace gpds
{

#define GPDS_ASSERT(x) \
        if ( !(x) ) {  \
            printf("GPDS Assertion!\n"); \
            printf("  Function : %s\n", __FUNCTION__); \
            printf("  File     : %s\n", __FILE__); \
            printf("  Line     : %d\n", __LINE__); \
            abort();  \
        }

    template<class T>
    struct is_c_str :
        std::integral_constant<
            bool,
            std::is_same<char const*, typename std::decay<T>::type>::value ||
            std::is_same<char*, typename std::decay<T>::type>::value
        >
    {
    };

    template<typename T>
    [[nodiscard]]
    static
    std::string
    value_to_string(const T& value) noexcept
    {
        using Td = std::decay_t<T>;

        try {
            // Bool
            if constexpr (std::is_same<Td, bool>::value)
                return value ? "true" : "false";

            // int
            else if constexpr (std::is_same<Td, int>::value)
                return std::to_string(value);

            // float
            else if constexpr (std::is_same<Td, float>::value) {
                std::string str = std::to_string(value);
                // Remove trailing zeros
                str.erase(str.find_last_not_of('0') + 1, std::string::npos);
                return str;
            }

            // double
            else if constexpr (std::is_same<Td, double>::value) {
                std::string str = std::to_string(value);
                // Remove trailing zeros
                str.erase(str.find_last_not_of('0') + 1, std::string::npos);
                return str;
            }

            // std::size_t
            else if constexpr (std::is_same<Td, std::size_t>::value)
                return std::to_string(value);

            // std::string
            else if constexpr (std::is_same<Td, std::string>::value)
                return value;

            // std::string_view
            else if constexpr (std::is_same<Td, std::string_view>::value)
                return std::string{value};

            // C-string
            else if constexpr (is_c_str<Td>::value)
                return std::string(value);

            // std::filesystem::path
            else if constexpr (std::is_same<Td, std::filesystem::path>::value)
                return value.string();
        }
        catch (...) {
            return { };
        }

        return { };
    }

    template<typename T>
    [[nodiscard]]
    static
    std::optional<T>
    string_to_value(const std::string& string) noexcept
    {
        using Td = std::decay_t<T>;

        try {
            // bool
            if constexpr (std::is_same<Td, bool>::value)
                return (string == "true");

            // int
            else if constexpr (std::is_same<Td, int>::value)
                return std::stoi(string);

            // float
            else if constexpr (std::is_same<Td, float>::value)
                return std::stof(string);

            // double
            else if constexpr (std::is_same<Td, double>::value)
                return std::stod(string);

            // std::size_t
            else if constexpr (std::is_same<Td, std::size_t>::value)
                return std::stoull(string);

            // std::string
            else if constexpr (std::is_same<Td, std::string>::value)
                return string;

            // std::filesystem::path
            else if constexpr (std::is_same<Td, std::filesystem::path>::value)
                return std::filesystem::path{ string };
        }
        catch (...) {
            return std::nullopt;
        }

        return std::nullopt;
    }
}
