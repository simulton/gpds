#include "../test.hpp"

TEST_CASE("retrieving wrong type from container")
{
    gpds::container container;

    SUBCASE("retrieving integer attribute as string") {
        container.add_attribute("number", 1234);
        auto str = container.get_attribute<std::string>("number");
        REQUIRE(str.has_value());
        REQUIRE(str.value() == "1234");
    }

    SUBCASE("retrieving string attribute as integer") {
        container.add_attribute("text", "1234");
        auto opt = container.get_attribute<int>("text");
        REQUIRE(opt.has_value());
        REQUIRE(opt.value() == 1234);
    }
}
