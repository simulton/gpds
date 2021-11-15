#include "../test.h"

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
}
