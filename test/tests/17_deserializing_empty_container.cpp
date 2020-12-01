#include <sstream>

#include "gpds/archiver_xml.hpp"
#include "../test.h"

static const std::string FILE_CONTENT =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
        "<data/>";

TEST_CASE("Empty elements are deserialized correctly")
{

    std::stringstream ss(FILE_CONTENT);
    gpds::container container;

    gpds::archiver_xml ar;
    ar.load(ss, container, "data");

    std::vector<std::string> vector;
    vector = container.get_values<std::string>("test");
    REQUIRE(vector.size() == 0);
}
