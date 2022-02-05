#include <cstring>
#include "../test.hpp"

TEST_SUITE("values")
{

    TEST_CASE("creation")
    {
        SUBCASE("integer") {
            gpds::value value(45);

            SUBCASE("retrieving integer from value") {
                REQUIRE(value.get<int>() == 45);
            }
        }

        SUBCASE("boolean") {
            gpds::value value(false);

            SUBCASE("retrieving boolean from value") {
                REQUIRE(value.get<bool>() == false);
            }
        }

        SUBCASE("floating point") {
            // ToDo
            /*
            SUBCASE ("float") {
                gpds::value value(13.2f);

                SUBCASE("retrieving floating-point from value") {
                    REQUIRE(value.get<float>().value_or(0.0f) == doctest::Approx(13.2f));
                }
            }
            */

            SUBCASE("double") {
                gpds::value value(13.2);

                SUBCASE("retrieving floating-point from value") {
                    REQUIRE(value.get<double>().value_or(0.0) == doctest::Approx(13.2));
                }
            }
        }

        SUBCASE("string") {
            SUBCASE("std::string") {
                gpds::value value(std::string("Hello, World!"));

                SUBCASE("retrieving string from value") {
                    REQUIRE(value.get<std::string>() == "Hello, World!");
                }
            }

            SUBCASE("c-string") {
                gpds::value value("Hello, World!");

                SUBCASE("retrieving string from value") {
                    REQUIRE(value.get<std::string>() == "Hello, World!");
                }
            }
        }

        SUBCASE("filesystem path")
        {
            gpds::value v(std::filesystem::path{ "/usr/src" });

            SUBCASE("retrieve string from value") {
                REQUIRE(v.get<std::filesystem::path>() == "/usr/src");
            }
        }

        SUBCASE("container") {
            gpds::container* container = new gpds::container;
            container->add_value("name", std::string("John Doe"));
            gpds::value value(container);

            SUBCASE("retrieving value from container") {
                auto ctnr = value.get<gpds::container*>().value_or(nullptr);
                REQUIRE(ctnr);
                REQUIRE(ctnr->get_value<std::string>("name") == "John Doe");
            }
        }

        SUBCASE("empty container") {
            gpds::value value;
            value.set(new gpds::container);

            const auto& opt = value.get<gpds::container*>();
            CHECK(opt.has_value());
            CHECK(opt.value_or(nullptr));
            CHECK(opt.value()->empty());
        }
    }

    TEST_CASE("default value")
    {
        gpds::container container;
        std::string str = container.get_value<std::string>("doesn't exist").value_or("default");
        CHECK_EQ(str, "default");
    }

}
