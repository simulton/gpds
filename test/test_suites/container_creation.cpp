#include "../test.hpp"

TEST_SUITE("container")
{
    TEST_CASE("creation")
    {
        gpds::container container;

        SUBCASE("adding values") {

            SUBCASE("adding an integer value") {
                container.add_value("integer", 1337);
                REQUIRE(container.get_value<int>("integer") == 1337);
            }

            SUBCASE("adding a text value") {
                container.add_value("text", std::string("lorem ipsum"));
                REQUIRE(container.get_value<std::string>("text") == "lorem ipsum");
            }

            SUBCASE("adding a boolean value") {
                container.add_value("boolean", false);
                REQUIRE(container.get_value<bool>("boolean") == false);
            }

            SUBCASE("adding a floating-point value") {
                container.add_value("float", 2.5);
                REQUIRE(container.get_value<double>("float") == doctest::Approx(2.5));
            }

            SUBCASE("adding a gpds value") {
                gpds::value v;
                v.add_attribute("a", "foo");
                v.add_attribute("b", "bar");
                v.set<std::string>("foobar");

                container.add_value("v", v);

                REQUIRE_EQ(container.get_value_attribute<std::string>("v", "a"), "foo");
                REQUIRE_EQ(container.get_value_attribute<std::string>("v", "b"), "bar");
            }
        }

        SUBCASE("adding attributes") {

            SUBCASE("adding an integer attribute") {
                container.add_attribute("integer", 1337);
                REQUIRE(container.get_attribute<int>("integer") == 1337);
            }

            SUBCASE("adding a text attribute") {
                container.add_attribute("text", std::string("lorem ipsum"));
                REQUIRE(container.get_attribute<std::string>("text") == "lorem ipsum");
            }

            SUBCASE("adding a boolean attribute") {
                container.add_attribute("boolean", false);
                REQUIRE(container.get_attribute<bool>("boolean") == false);
            }

            SUBCASE("adding a floating-point attribute") {
                container.add_attribute("float", 2.5);
                REQUIRE(container.get_attribute<double>("float") == doctest::Approx(2.5));
            }
        }
    }
}