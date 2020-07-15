#include <sstream>
#include "doctest.h"
#include "../test.h"
#include "gpds/serialize.hpp"
#include "color.h"

TEST_CASE("Color class")
{
    // Create a color
    gpds_test::color red1;
    red1.name = "Red";
    red1.red = 255;
    red1.green = 0;
    red1.blue = 0;

    // Serialize
    std::stringstream serialized;
    REQUIRE(gpds_test::test::serialize(serialized, red1, "color"));

    // Deserialize
    gpds_test::color red2;
    REQUIRE(gpds_test::test::deserialize(serialized, red2, "color"));

    // Ensure that data is the same
    REQUIRE(red1 == red2);
}

