#include <sstream>
#include "catch2/catch.hpp"
#include "../test.h"
#include "serialize.h"
#include "color.h"

TEST_CASE( "Color class" )
{
    // Create a color
    GpdsTest::Color red1;
    red1.name = "Red";
    red1.red = 255;
    red1.green = 0;
    red1.blue = 0;

    // Serialize
    std::stringstream serialized;
    REQUIRE( GpdsTest::Test::serialize( serialized, red1, "color" ) );

    // Deserialize
    GpdsTest::Color red2;
    REQUIRE( GpdsTest::Test::deserialize( serialized, red2, "color" ) );

    // Ensure that data is the same
    REQUIRE( red1 == red2 );
}

