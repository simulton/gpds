#include <cstring>
#include "doctest.h"
#include "../test.h"
#include "serialize.hpp"

TEST_CASE("values can be created")
{
    SUBCASE("creating an integer value") {
        gpds::value value(45);
        REQUIRE(value.is_type<int>());

        SUBCASE("retrieving integer from value") {
            REQUIRE(value.get<int>() == 45);
        }
    }

    SUBCASE("creating an boolean value") {
        gpds::value value(false);
        REQUIRE(value.is_type<bool>());

        SUBCASE("retrieving boolean from value") {
            REQUIRE(value.get<bool>() == false);
        }
    }

    SUBCASE("creating an floating-point value") {
        SUBCASE ("from a float") {
            gpds::value value(13.2f);
            REQUIRE(value.is_type<double>());

            SUBCASE("retrieving floating-point from value") {
                REQUIRE(value.get<double>() == doctest::Approx(13.2f));
            }
        }

        SUBCASE("from a double") {
            gpds::value value(13.2);
            REQUIRE(value.is_type<double>());

            SUBCASE("retrieving floating-point from value") {
                REQUIRE(value.get<double>() == doctest::Approx(13.2));
            }
        }
    }

    SUBCASE("creating a string value") {
        SUBCASE("from an std::string") {
            gpds::value value(std::string("Hello, World!"));
            REQUIRE(value.is_type<std::string>());

            SUBCASE("retrieving string from value") {
                REQUIRE(value.get<std::string>() == "Hello, World!");
            }
        }

        SUBCASE("from a c-string") {
            gpds::value value("Hello, World!");
            REQUIRE(value.is_type<std::string>());

            SUBCASE("retrieving string from value") {
                REQUIRE(value.get<std::string>() == "Hello, World!");
            }
        }
    }

    SUBCASE("creating a container") {
        gpds::container* container = new gpds::container;
        container->add_value("name", std::string("John Doe"));
        gpds::value value(container);
        REQUIRE(strcmp(value.type_string(), "nested") == 0);

        SUBCASE("retrieving value from container") {
            auto ctnr = value.get<gpds::container*>();
            REQUIRE(ctnr);
            REQUIRE(ctnr->get_value<std::string>("name") == "John Doe");
        }
    }
}

