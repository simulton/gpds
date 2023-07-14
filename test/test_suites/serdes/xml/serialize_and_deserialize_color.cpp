#include "../../test.hpp"
#include "../../color.h"

TEST_SUITE("serdes - xml")
{

    TEST_CASE("color class")
    {
        // Create a color
        gpds_test::color red1;
        red1.name = "Red";
        red1.red = 255;
        red1.green = 0;
        red1.blue = 0;

        // Serialize
        std::stringstream serialized;
        gpds_test::serialize<gpds::archiver_xml>(serialized, red1, "color");

        // Deserialize
        gpds_test::color red2;
        gpds_test::deserialize<gpds::archiver_xml>(serialized, red2, "color");

        // Ensure that data is the same
        CHECK_EQ(red1, red2);
    }

}
