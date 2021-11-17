#include <cstring>
#include "../test.hpp"

TEST_SUITE("values")
{

    TEST_CASE("creation")
    {
        SUBCASE("integer") {
            gpds::value value(45);
            REQUIRE(value.is_type<int>());

            SUBCASE("retrieving integer from value") {
                REQUIRE(value.get<int>() == 45);
            }
        }

        SUBCASE("boolean") {
            gpds::value value(false);
            REQUIRE(value.is_type<bool>());

            SUBCASE("retrieving boolean from value") {
                REQUIRE(value.get<bool>() == false);
            }
        }

        SUBCASE("floating point") {
            SUBCASE ("float") {
                gpds::value value(13.2f);
                REQUIRE(value.is_type<double>());

                SUBCASE("retrieving floating-point from value") {
                    REQUIRE(value.get<double>() == doctest::Approx(13.2f));
                }
            }

            SUBCASE("double") {
                gpds::value value(13.2);
                REQUIRE(value.is_type<double>());

                SUBCASE("retrieving floating-point from value") {
                    REQUIRE(value.get<double>() == doctest::Approx(13.2));
                }
            }
        }

        SUBCASE("string") {
            SUBCASE("std::string") {
                gpds::value value(std::string("Hello, World!"));
                REQUIRE(value.is_type<std::string>());

                SUBCASE("retrieving string from value") {
                    REQUIRE(value.get<std::string>() == "Hello, World!");
                }
            }

            SUBCASE("c-string") {
                gpds::value value("Hello, World!");
                REQUIRE(value.is_type<std::string>());

                SUBCASE("retrieving string from value") {
                    REQUIRE(value.get<std::string>() == "Hello, World!");
                }
            }
        }

        SUBCASE("container") {
            gpds::container *container = new gpds::container;
            container->add_value("name", std::string("John Doe"));
            gpds::value value(container);
            REQUIRE(strcmp(value.type_string(), "nested") == 0);

            SUBCASE("retrieving value from container") {
                auto ctnr = value.get<gpds::container *>();
                REQUIRE(ctnr);
                REQUIRE(ctnr->get_value<std::string>("name") == "John Doe");
            }
        }
    }

    TEST_CASE("default value")
    {
        gpds::container container;
        std::string str = container.get_value<std::string>("doesn't exist").value_or("default");
        CHECK_EQ(str, "default");
    }

}
