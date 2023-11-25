#include "../../test.hpp"

TEST_SUITE("core - values")
{
    TEST_CASE("to string")
    {
        SUBCASE("boolean")
        {
            bool value = true;
            std::string str = gpds::value_to_string(value);
            CHECK_EQ(str, "true");
        }

        SUBCASE("int")
        {
            int value = 10;
            std::string str = gpds::value_to_string(value);
            CHECK_EQ(str, "10");
        }

        SUBCASE("float")
        {
            float value = 1.25f;
            std::string str = gpds::value_to_string(value);
            CHECK_EQ(str, "1.25");
        }

        SUBCASE("double")
        {
            double value = 1.25;
            std::string str = gpds::value_to_string(value);
            CHECK_EQ(str, "1.25");
        }

        SUBCASE("string")
        {
            std::string value = "F82GA3B";
            std::string str = gpds::value_to_string(value);
            CHECK_EQ(str, "F82GA3B");
        }

        SUBCASE("c-string")
        {
            const char* value = "E2FEB55";
            std::string str = gpds::value_to_string(value);
            CHECK_EQ(str, "E2FEB55");
        }

        SUBCASE("invalid")
        {
            std::vector<std::string> vec;
            std::string str = gpds::value_to_string(vec);
            CHECK(str.empty());
        }
    }

    TEST_CASE("from string")
    {
        SUBCASE("boolean")
        {
            std::string str = "false";
            auto value = gpds::string_to_value<bool>(str);
            REQUIRE(value.has_value());
            CHECK_EQ(value.value(), false);
        }

        SUBCASE("integer (positive)")
        {
            std::string str = "12345";
            auto value = gpds::string_to_value<int>(str);
            REQUIRE(value.has_value());
            CHECK_EQ(value.value(), 12345);
        }

        SUBCASE("integer (negative)")
        {
            std::string str = "-1234";
            auto value = gpds::string_to_value<int>(str);
            REQUIRE(value.has_value());
            CHECK_EQ(value.value(), -1234);
        }

        SUBCASE("float (positive)")
        {
            std::string str = "7.9433355";
            auto value = gpds::string_to_value<float>(str);
            REQUIRE(value.has_value());
            CHECK_EQ(value.value(), doctest::Approx(7.9433355));
        }

        SUBCASE("float (negative)")
        {
            std::string str = "-82.7777779";
            auto value = gpds::string_to_value<float>(str);
            REQUIRE(value.has_value());
            CHECK_EQ(value.value(), doctest::Approx(-82.7777779f));
        }

        SUBCASE("double (positive)")
        {
            std::string str = "1.33333333";
            auto value = gpds::string_to_value<double>(str);
            REQUIRE(value.has_value());
            CHECK_EQ(value.value(), doctest::Approx(1.33333333));
        }

        SUBCASE("double (negative)")
        {
            std::string str = "-1.33333333";
            auto value = gpds::string_to_value<double>(str);
            REQUIRE(value.has_value());
            CHECK_EQ(value.value(), doctest::Approx(-1.33333333));
        }

        SUBCASE("string")
        {
            std::string str = "R8BB45C";
            auto value = gpds::string_to_value<std::string>(str);
            REQUIRE(value.has_value());
            CHECK_EQ(value.value(), str);
        }
    }
}
