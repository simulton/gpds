#include "doctest.h"
#include "../test.h"
#include "serialize.h"
#include <iostream>

TEST_CASE( "containers can be created" )
{
    Gpds::Container container;

    SUBCASE( "adding values" ) {

        SUBCASE( "adding an integer value" ) {
            container.addValue("integer", 1337);
            REQUIRE(container.getValue<int>("integer") == 1337);
        }

        SUBCASE( "adding a text value" ) {
            container.addValue("text", std::string("lorem ipsum"));
            REQUIRE(container.getValue<std::string>("text") == "lorem ipsum");
        }

        SUBCASE( "adding a boolean value" ) {
            container.addValue("boolean", false);
            REQUIRE(container.getValue<bool>("boolean") == false);
        }

        SUBCASE( "adding a floating-point value" ) {
            container.addValue("float", 2.5);
            REQUIRE(container.getValue<double>("float") == doctest::Approx(2.5));
        }
    }

    SUBCASE( "adding attributes" ) {

        SUBCASE( "adding an integer attribute" ) {
            container.addAttribute("integer", 1337);
            REQUIRE(container.getAttribute<int>("integer") == 1337);
        }

        SUBCASE( "adding a text attribute" ) {
            container.addAttribute("text", std::string("lorem ipsum"));
            REQUIRE(container.getAttribute<std::string>("text") == "lorem ipsum");
        }

        SUBCASE( "adding a boolean attribute" ) {
            container.addAttribute("boolean", false);
            REQUIRE(container.getAttribute<bool>("boolean") == false);
        }

        SUBCASE( "adding a floating-point attribute" ) {
            container.addAttribute("float", 2.5);
            REQUIRE(container.getAttribute<double>("float") == doctest::Approx(2.5));
        }
    }

    SUBCASE( "adding a comment" ) {
        container.setComment("comment");
        REQUIRE(container.comment == "comment");
    }
}
