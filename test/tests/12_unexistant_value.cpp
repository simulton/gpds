#include "../test.h"
#include "gpds/container.hpp"

TEST_CASE("unexistant value returns default value")
{
    gpds::container container;
    std::string str = container.get_value<std::string>("doesn't exist").value_or("default");
    REQUIRE(str == "default");
}

