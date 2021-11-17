#include "../test.hpp"

TEST_CASE("values can be converted to strings")
{
    SUBCASE("converting a boolean to a string") {
        bool value = true;
        std::string str = gpds::value_to_string(value);
        REQUIRE(str == "true");
    }

    SUBCASE("converting an integer to a string") {
        int value = 10;
        std::string str = gpds::value_to_string(value);
        REQUIRE(str == "10");
    }

    SUBCASE("converting a double to a string") {
        double value = 1.25;
        std::string str = gpds::value_to_string(value);
        REQUIRE(str == "1.25");
    }

    SUBCASE("converting a string to a string") {
        std::string value = "F82GA3B";
        std::string str = gpds::value_to_string(value);
        REQUIRE(str == "F82GA3B");
    }

    SUBCASE("converting a c-string to a string") {
        const char* value = "E2FEB55";
        std::string str = gpds::value_to_string(value);
        REQUIRE(str == "E2FEB55");
    }

    SUBCASE("converting an invalid type to a string") {
        std::vector<std::string> vec;
        std::string str = gpds::value_to_string(vec);
        REQUIRE(str.empty());
    }
}

TEST_CASE("strings can be converted back into values")
{
    SUBCASE("converting a string to a boolean") {
        std::string str = "false";
        auto value = gpds::string_to_value<bool>(str);
        REQUIRE(value.has_value());
        REQUIRE(value.value() == false);
    }

    SUBCASE("converting a string to a integer") {
        std::string str = "12345";
        auto value = gpds::string_to_value<int>(str);
        REQUIRE(value.has_value());
        REQUIRE(value.value() == 12345);
    }

    SUBCASE("converting a string containing a negative number to an integer") {
        std::string str = "-1234";
        auto value = gpds::string_to_value<int>(str);
        REQUIRE(value.has_value());
        REQUIRE(value.value() == -1234);
    }

    SUBCASE("converting a string to a double") {
        std::string str = "1.33333333";
        auto value = gpds::string_to_value<double>(str);
        REQUIRE(value.has_value());
        REQUIRE(value.value() == doctest::Approx(1.33333333));
    }

    SUBCASE("converting a string containing a negative decimal to a double") {
        std::string str = "-1.33333333";
        auto value = gpds::string_to_value<double>(str);
        REQUIRE(value.has_value());
        REQUIRE(value.value() == doctest::Approx(-1.33333333));
    }

    SUBCASE("converting a string to a string") {
        std::string str = "R8BB45C";
        auto value = gpds::string_to_value<std::string>(str);
        REQUIRE(value.has_value());
        REQUIRE(value.value() == str);
    }
}
