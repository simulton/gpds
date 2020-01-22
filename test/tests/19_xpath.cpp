#include <iostream>
#include "doctest.h"
#include "container.hpp"
#include "archiver_xml.hpp"

#ifdef GPDS_FEATURE_XPATH

static const std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<data>\n"
    "    <boolean>true</boolean>\n"
    "    <boolean id=\"1234\">false</boolean>\n"
    "</data>\n";

TEST_CASE("Query document with XPath")
{
    gpds::archiver_xml ar;
    std::stringstream stream(FILE_CONTENT);
    std::unique_ptr<gpds::document> doc = ar.load(stream);
    REQUIRE(doc);

    SUBCASE("Query boolean as string") {
        std::string value = doc->query("data/boolean");
        REQUIRE(value == "true");
    }

    SUBCASE("Query boolean") {
        gpds::value value = doc->query_value("data/boolean");
        REQUIRE(value.is_type<bool>());
        std::optional<bool> optional = value.get<bool>();
        REQUIRE(optional.has_value());
        REQUIRE(optional.value() == true);
    }

    SUBCASE("Query attribute") {
        std::string value = doc->query("data/boolean[@id='1234']");
        REQUIRE(value == "false");
    }

    SUBCASE("Query container as string") {
        std::string value = doc->query("data");
        REQUIRE(value == FILE_CONTENT.substr(FILE_CONTENT.find('\n') + 1));
    }

    SUBCASE("Query container") {
        gpds::value value = doc->query_value("data");
        REQUIRE(value.is_type<gpds::container*>());
        gpds::container* container = value.get<gpds::container*>();
        REQUIRE(container->get_value<bool>("boolean").has_value());
    }

    SUBCASE("Query an nonexistent element") {
        std::string value = doc->query("data/this_doesnt_exist");
        REQUIRE(value.empty());
    }
}

#endif
