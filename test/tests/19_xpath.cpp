#include <iostream>
#include "doctest.h"
#include "container.hpp"
#include "archiver_xml.hpp"

#ifdef GPDS_FEATURE_XPATH

static const std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<data>"
    "  <boolean>true</boolean>"
    "  <boolean id=\"1234\">false</boolean>"
    "</data>";

TEST_CASE("Query document with XPath")
{
    gpds::archiver_xml ar;
    std::stringstream stream(FILE_CONTENT);
    gpds::document* doc = ar.load(stream);

    SUBCASE("Query boolean as string") {
        std::string value = doc->query("data/boolean");
        REQUIRE(value == "true");
    }

    SUBCASE("Query attribute") {
        std::string value = doc->query("data/boolean[@id='1234']");
        REQUIRE(value == "false");
    }

    SUBCASE("Query container") {
        gpds::value value = doc->query_value("data");
        REQUIRE(value.is_type<gpds::container*>());
        gpds::container* container = value.get<gpds::container*>();
        REQUIRE(container->get_value<bool>("boolean").has_value());
    }
}

#endif
