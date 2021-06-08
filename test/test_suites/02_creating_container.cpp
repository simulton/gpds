#include "../test.h"
#include "gpds/serialize.hpp"
#include <iostream>

TEST_CASE("containers can be created")
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
