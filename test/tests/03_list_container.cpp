#include "catch2/catch.hpp"
#include "../test.h"
#include "serialize.h"

TEST_CASE( "container can contain a list of elements" )
{
    Gpds::Container container;

    SECTION( "creating list container" ) {
        for (int i = 0; i < 10; i++) {
            container.addValue("list-item", i);
        }

        REQUIRE(container.isList());

        std::vector<int> list = container.getValues<int>("list-item");

        REQUIRE(list.size() == 10);
    }
}
