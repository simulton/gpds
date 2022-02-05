#pragma once

#include <filesystem>
#include <optional>

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
        > {};

    template<typename T>
    [[nodiscard]]
    static
    std::string
    value_to_string(const T& value) noexcept
    {
        try {
            // Bool
            if constexpr (std::is_same<T, bool>::value)
                return value ? "true" : "false";

            // int
            else if constexpr (std::is_same<T, int>::value)
                return std::to_string(value);

            // double
            else if constexpr (std::is_same<T, double>::value) {
                std::string str = std::to_string(value);
                // Remove trailing zeros
                str.erase(str.find_last_not_of('0') + 1, std::string::npos);
                return str;
            }

            // std::string
            else if constexpr (std::is_same<T, std::string>::value)
                return value;

            // C-string
            else if constexpr (is_c_str<T>::value)
                return std::string(value);

            // std::filesystem::path
            else if constexpr (std::is_same<T, std::filesystem::path>::value)
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
        try {
            // bool
            if constexpr (std::is_same<T, bool>::value)
                return (string == "true");

            // int
            else if constexpr (std::is_same<T, int>::value)
                return std::stoi(string);

            // double
            else if constexpr (std::is_same<T, double>::value)
                return std::stod(string);

            // std::string
            else if constexpr (std::is_same<T, std::string>::value)
                return string;

            // std::filesystem::path
            else if constexpr (std::is_same<T, std::filesystem::path>::value)
                return std::filesystem::path{ string };
        }
        catch (...) {
            return std::nullopt;
        }

        return std::nullopt;
    }
}
