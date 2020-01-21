#include <iostream>
#include "doctest.h"
#include "container.hpp"
#include "archiver_xml.hpp"

#ifdef GPDS_FEATURE_XPATH

static const std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<data>"
    "  <boolean>true</boolean>"
    "  <boolean>false</boolean>"
    "</data>";

TEST_CASE("Query document with XPath")
{
    gpds::archiver_xml ar;
    std::stringstream stream(FILE_CONTENT);

    gpds::document* doc = ar.load(stream);
    gpds::value value = doc->query("data/boolean");
    REQUIRE(value.is_type<bool>());
    std::optional<bool> valueOpt = value.get<bool>();
    REQUIRE(valueOpt.has_value());
    REQUIRE(valueOpt.value() == true);
}

#endif
