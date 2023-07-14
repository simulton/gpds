#include <sstream>

#include "gpds/archiver_yaml.hpp"
#include "../../test.hpp"

static const std::string FILE_CONTENT_1 =
R"""(
---
data
)""";

static const std::string FILE_CONTENT_2 =
R"""(
---
data: 
  test
)""";

static const std::string FILE_CONTENT_3 =
R"""(
---
data: 
  test:
    value: 42
)""";

TEST_SUITE("serdes - yaml")
{

    TEST_CASE("Empty elements are deserialized correctly (1)")
    {
        std::stringstream ss(FILE_CONTENT_1);
        gpds::container container;

        gpds::archiver_yaml ar;
        ar.load(ss, container, "data");

        std::vector<std::string> vector;
        vector = container.get_values<std::string>("test");
        REQUIRE(vector.size() == 0);
    }

    TEST_CASE("Empty elements are deserialized correctly (2)")
    {
        std::stringstream ss(FILE_CONTENT_2);
        gpds::container container;

        gpds::archiver_yaml ar;
        ar.load(ss, container, "data");

        for (const gpds::container* cc : container.get_values<gpds::container*>("test")) {
            REQUIRE(cc);
        }
    }

    TEST_CASE("Empty elements are deserialized correctly (3)")
    {
        std::stringstream ss(FILE_CONTENT_3);
        gpds::container container;

        gpds::archiver_yaml ar;
        ar.load(ss, container, "data");

        for (const gpds::container* cc : container.get_values<gpds::container*>("test")) {
            REQUIRE(cc);
            CHECK_EQ(cc->get_value<int>("value").value_or(-1), 42);
        }
    }

}
