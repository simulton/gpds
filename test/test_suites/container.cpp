#include "../test.hpp"

TEST_SUITE("container")
{
    TEST_CASE("inspection")
    {

        gpds::container c;
        CHECK(c.empty());
        CHECK_FALSE(c.has_values());
        CHECK_FALSE(c.has_attributes());

        SUBCASE("attribute only")
        {
            c.add_attribute("foo", "bar");

            CHECK_FALSE(c.empty());
            CHECK_FALSE(c.has_values());
            CHECK(c.has_attributes());
        }

        SUBCASE("value only")
        {
            c.add_value("v", "k");

            CHECK_FALSE(c.empty());
            CHECK(c.has_values());
            CHECK_FALSE(c.has_attributes());
        }

        SUBCASE("attribute and value")
        {
            c.add_attribute("foo", "bar");
            c.add_value("v", "k");

            CHECK_FALSE(c.empty());
            CHECK(c.has_values());
            CHECK(c.has_attributes());
        }

    }

    TEST_CASE("get gpds::value")
    {
        static constexpr const char* desc_short = "foobar";
        static constexpr const char* desc_long  = "This is a longer description but still something about foobar";

        gpds::container c;
        c.add_value("description", "foobar").add_attribute("type", "short");
        c.add_value("description", "This is a longer description but still foobar").add_attribute("type", "long");


    }

    TEST_CASE("get multiple gpds::value")
    {
        static constexpr const char* desc_short = "foobar";
        static constexpr const char* desc_long  = "This is a longer description but still something about foobar";

        gpds::container c;
        c.add_value("description", desc_short).add_attribute("type", "short");
        c.add_value("description", desc_long).add_attribute("type", "long");

        const auto& values = c.get_values<gpds::value>("description");
        CHECK_EQ(values.size(), 2);

        // Check description short
        CHECK(values.at(0).is_type<std::string>());
        CHECK_EQ(values.at(0).get_attribute<std::string>("type").value_or(""), "short");
        CHECK_EQ(values.at(0).get<std::string>(), desc_short);

        // Check description long
        CHECK(values.at(1).is_type<std::string>());
        CHECK_EQ(values.at(1).get_attribute<std::string>("type").value_or(""), "long");
        CHECK_EQ(values.at(1).get<std::string>(), desc_long);
    }
}
