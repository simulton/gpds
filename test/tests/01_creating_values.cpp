#include <cstring>
#include "doctest.h"
#include "../test.h"
#include "serialize.h"

TEST_CASE( "values can be created" )
{
    SUBCASE( "creating an integer value" ) {
        Gpds::Value value(45);
        REQUIRE(value.isType<int>());

        SUBCASE( "retrieving integer from value" ) {
            REQUIRE(value.get<int>() == 45);
        }
    }

    SUBCASE( "creating an boolean value" ) {
        Gpds::Value value(false);
        REQUIRE(value.isType<bool>());

        SUBCASE( "retrieving boolean from value" ) {
            REQUIRE(value.get<bool>() == false);
        }
    }

    SUBCASE( "creating an floating-point value" ) {
        SUBCASE ( "from a float" ) {
            Gpds::Value value(13.2f);
            REQUIRE(value.isType<double>());

            SUBCASE( "retrieving floating-point from value" ) {
                REQUIRE(value.get<double>() == doctest::Approx(13.2f));
            }
        }

        SUBCASE ( "from a double" ) {
            Gpds::Value value(13.2);
            REQUIRE(value.isType<double>());

            SUBCASE( "retrieving floating-point from value" ) {
                REQUIRE(value.get<double>() == doctest::Approx(13.2));
            }
        }
    }

    SUBCASE( "creating a string value" ) {
        SUBCASE( "from an std::string" ) {
            Gpds::Value value(std::string("Hello, World!"));
            REQUIRE(value.isType<std::string>());

            SUBCASE( "retrieving string from value" ) {
                REQUIRE(value.get<std::string>() == "Hello, World!");
            }
        }

        SUBCASE( "from a c-string" ) {
            Gpds::Value value("Hello, World!");
            REQUIRE(value.isType<std::string>());

            SUBCASE( "retrieving string from value" ) {
                REQUIRE(value.get<std::string>() == "Hello, World!");
            }
        }
    }

    SUBCASE( "creating a container" ) {
        Gpds::Container* container = new Gpds::Container;
        container->addValue("name", std::string("John Doe"));
        Gpds::Value value(container);
        REQUIRE( strcmp( value.typeString(), "nested" ) == 0 );

        SUBCASE( "retrieving value from container" ) {
            auto ctnr = value.get<Gpds::Container*>();
            REQUIRE(ctnr);
            REQUIRE(ctnr->getValue<std::string>("name") == "John Doe");
        }
    }
}

