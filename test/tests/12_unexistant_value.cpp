#include "doctest.h"
#include "../test.h"
#include "container.h"

TEST_CASE("unexistant value returns default value")
{
    gpds::container container;
    std::string str = container.get_value<std::string>("doesn't exist", "default");
    REQUIRE(str == "default");
}

