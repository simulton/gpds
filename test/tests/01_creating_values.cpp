#include "catch2/catch.hpp"
#include "../test.h"
#include "serialize.h"

TEST_CASE( "values can be created", "[Value]" )
{
    SECTION( "creating an integer value" ) {
        Gpds::Value value(45);
        REQUIRE(value.isType<int>());

        SECTION( "retrieving integer from value" ) {
            REQUIRE(value.get<int>() == 45);
        }
    }

    SECTION( "creating an boolean value" ) {
        Gpds::Value value(false);
        REQUIRE(value.isType<bool>());

        SECTION( "retrieving boolean from value" ) {
            REQUIRE(value.get<bool>() == false);
        }
    }

    SECTION( "creating an floating-point value" ) {
        SECTION ( "from a float" ) {
            Gpds::Value value(13.2f);
            REQUIRE(value.isType<double>());

            SECTION( "retrieving floating-point from value" ) {
                REQUIRE(value.get<double>() == Approx(13.2f));
            }
        }

        SECTION ( "from a double" ) {
            Gpds::Value value(13.2);
            REQUIRE(value.isType<double>());

            SECTION( "retrieving floating-point from value" ) {
                REQUIRE(value.get<double>() == Approx(13.2));
            }
        }
    }

    SECTION( "creating a string value" ) {
        SECTION( "from an std::string" ) {
            Gpds::Value value(std::string("Hello, World!"));
            REQUIRE(value.isType<std::string>());

            SECTION( "retrieving string from value" ) {
                REQUIRE(value.get<std::string>() == "Hello, World!");
            }
        }

        SECTION( "from a c-string" ) {
            Gpds::Value value("Hello, World!");
            REQUIRE(value.isType<std::string>());

            SECTION( "retrieving string from value" ) {
                REQUIRE(value.get<std::string>() == "Hello, World!");
            }
        }
    }

    SECTION( "creating a container" ) {
        Gpds::Container* container = new Gpds::Container;
        container->addValue("name", std::string("John Doe"));
        Gpds::Value value(container);
        REQUIRE(value.typeString() == "nested");

        SECTION( "retrieving value from container" ) {
            auto ctnr = value.get<Gpds::Container*>();
            REQUIRE(ctnr);
            REQUIRE(ctnr->getValue<std::string>("name") == "John Doe");
        }
    }
}

