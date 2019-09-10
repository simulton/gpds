#include "doctest.h"
#include "../test.h"
#include "container.h"

TEST_CASE( "unexistant value returns default value" )
{
    Gpds::Container container;
    std::string str = container.getValue<std::string>("doesn't exist", "default");
    REQUIRE(str == "default");
}

