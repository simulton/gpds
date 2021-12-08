#include <sstream>

#include "gpds/archiver_xml.hpp"
#include "../test.hpp"

static const std::string FILE_CONTENT_1 =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
        "<data/>";

static const std::string FILE_CONTENT_2 =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<data>\n"
    "  <test/>\n"
    "</data>";

static const std::string FILE_CONTENT_3 =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<data>\n"
    "  <test>\n"
    "    <value>42</value>\n"
    "  </test>\n"
    "</data>";

TEST_CASE("Empty elements are deserialized correctly (1)")
{
    std::stringstream ss(FILE_CONTENT_1);
    gpds::container container;

    gpds::archiver_xml ar;
    ar.load(ss, container, "data");

    std::vector<std::string> vector;
    vector = container.get_values<std::string>("test");
    REQUIRE(vector.size() == 0);
}

TEST_CASE("Empty elements are deserialized correctly (2)")
{
    std::stringstream ss(FILE_CONTENT_2);
    gpds::container container;

    gpds::archiver_xml ar;
    ar.load(ss, container, "data");

    for (const gpds::container* cc : container.get_values<gpds::container*>("test")) {
        REQUIRE(cc);
    }
}

TEST_CASE("Empty elements are deserialized correctly (3)")
{
    std::stringstream ss(FILE_CONTENT_3);
    gpds::container container;

    gpds::archiver_xml ar;
    ar.load(ss, container, "data");

    for (const gpds::container* cc : container.get_values<gpds::container*>("test")) {
        REQUIRE(cc);
        CHECK_EQ(cc->get_value<int>("value").value_or(-1), 42);
    }
}