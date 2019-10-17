#include "doctest.h"
#include "container.h"
#include "archiverxml.h"
#include <sstream>

static const std::string FILE_CONTENT =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
        "<data/>";

TEST_CASE( "Empty elements are deserialized correctly" ) {

    std::stringstream ss(FILE_CONTENT);
    Gpds::Container container;

    Gpds::ArchiverXml ar;
    ar.load(ss, container, "data");

    std::vector<std::string> vector;
    vector = container.getValues<std::string>("test");
    REQUIRE(vector.size() == 0);
}
