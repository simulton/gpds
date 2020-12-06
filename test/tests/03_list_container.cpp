#include "../test.h"
#include "gpds/serialize.hpp"

TEST_CASE("container can contain a list of elements")
{
    gpds::container container;

    SUBCASE("creating list container") {
        for (int i = 0; i < 10; i++) {
            container.add_value("list-item", i);
        }

        REQUIRE(container.is_list());

        std::vector<int> list = container.get_values<int>("list-item");

        REQUIRE(list.size() == 10);
    }
}
