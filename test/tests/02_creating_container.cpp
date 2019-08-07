#include "catch2/catch.hpp"
#include "../test.h"
#include "serialize.h"
#include <iostream>

TEST_CASE( "containers can be created" )
{
    Gpds::Container container;

    SECTION( "adding values" ) {

        SECTION( "adding an integer value" ) {
            container.addValue("integer", 1337);
            REQUIRE(container.getValue<int>("integer") == 1337);
        }

        SECTION( "adding a text value" ) {
            container.addValue("text", std::string("lorem ipsum"));
            REQUIRE(container.getValue<std::string>("text") == "lorem ipsum");
        }

        SECTION( "adding a boolean value" ) {
            container.addValue("boolean", false);
            REQUIRE(container.getValue<bool>("boolean") == false);
        }

        SECTION( "adding a floating-point value" ) {
            container.addValue("float", 2.5);
            REQUIRE(container.getValue<double>("float") == Approx(2.5));
        }
    }

    SECTION( "adding attributes" ) {

        SECTION( "adding an integer attribute" ) {
            container.addAttribute("integer", 1337);
            REQUIRE(container.getAttribute<int>("integer") == 1337);
        }

        SECTION( "adding a text attribute" ) {
            container.addAttribute("text", std::string("lorem ipsum"));
            REQUIRE(container.getAttribute<std::string>("text") == "lorem ipsum");
        }

        SECTION( "adding a boolean attribute" ) {
            container.addAttribute("boolean", false);
            REQUIRE(container.getAttribute<bool>("boolean") == false);
        }

        SECTION( "adding a floating-point attribute" ) {
            container.addAttribute("float", 2.5);
            REQUIRE(container.getAttribute<double>("float") == Approx(2.5));
        }
    }

    SECTION( "adding a comment" ) {
        container.setComment("comment");
        REQUIRE(container.comment == "comment");
    }
}
