#include "../../test.hpp"

TEST_SUITE("core - container")
{
    TEST_CASE("lookup")
    {
        gpds::container c;

        SUBCASE("empty") {

        }

        SUBCASE("values")
        {
            SUBCASE("none")
            {
                CHECK(c.empty());
                CHECK_FALSE(c.has_values());
                CHECK_FALSE(c.has_attributes());
                CHECK_FALSE(c.has_value(""));
                CHECK_FALSE(c.has_value("foo"));
                CHECK_FALSE(c.has_attribute(""));
                CHECK_FALSE(c.has_attribute("foo"));
            }

            SUBCASE("one")
            {
                c.add_value("foo", "bar");

                CHECK_FALSE(c.empty());
                CHECK(c.has_values());
                CHECK_FALSE(c.has_attributes());

                CHECK(c.has_value("foo"));
                CHECK_FALSE(c.has_value("nonexist"));
                CHECK_FALSE(c.has_attribute("foo"));
                CHECK_FALSE(c.has_attribute("nonexist"));
            }

            SUBCASE("multiple")
            {
                c.add_value("foo", "bar");
                c.add_value("foo", "bar");
                c.add_value("foo", "bar");
                c.add_value("foobar", "zbar");
                c.add_value("some_number" , 4);

                CHECK_FALSE(c.empty());
                CHECK(c.has_values());
                CHECK_FALSE(c.has_attributes());

                CHECK(c.has_value("foo"));
                CHECK_EQ(c.get_values<std::string>("foo").size(), 3);

                CHECK(c.has_value("some_number"));
                CHECK_EQ(c.get_values<int>("some_number").size(), 1);
            }
        }

        SUBCASE("attributes")
        {
            SUBCASE("node")
            {
                CHECK(c.empty());
                CHECK_FALSE(c.has_values());
                CHECK_FALSE(c.has_attributes());
                CHECK_FALSE(c.has_value(""));
                CHECK_FALSE(c.has_value("foo"));
                CHECK_FALSE(c.has_attribute(""));
                CHECK_FALSE(c.has_attribute("foo"));
            }

            SUBCASE("one")
            {
                c.add_attribute("foo", "bar");

                CHECK_FALSE(c.empty());
                CHECK_FALSE(c.has_values());
                CHECK(c.has_attributes());

                CHECK(c.has_attribute("foo"));
            }
        }
    }
}
