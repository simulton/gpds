#include "gpds/document_xml.hpp"
#include "gpds/archiver_xml.hpp"
#include "../test.h"

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

    SUBCASE("Select a node that match the selection no matter where it is") {
        std::string value = doc->query("//boolean");
        REQUIRE(value == "true");
    }
}

TEST_CASE("Query fragments with XPath")
{
    gpds::archiver_xml ar;
    std::stringstream stream(FILE_CONTENT);
    std::unique_ptr<gpds::document> doc = ar.load(stream);
    REQUIRE(doc);

    SUBCASE("Retrieve a fragment and query it") {
        std::unique_ptr<gpds::fragment> fragment = doc->query_fragment("/data");
        REQUIRE(fragment);
        std::string value = fragment->query("boolean");
        REQUIRE(value == "true");
    }

    SUBCASE("Convert a fragment to a container") {
        std::unique_ptr<gpds::fragment> fragment = doc->query_fragment("/data");
        gpds::container container = fragment->to_container();
        std::optional<bool> optional = container.get_value<bool>("boolean");
        REQUIRE(optional.has_value());
        bool value = optional.value();
        REQUIRE(value == true);
    }

    SUBCASE("Select a fragment's parent") {
        std::unique_ptr<gpds::fragment> fragment = doc->query_fragment("/data/boolean");
        std::string value = fragment->query("..");
        REQUIRE(value == FILE_CONTENT.substr(FILE_CONTENT.find('\n') + 1));
    }
}

TEST_CASE("Query multiple elements") {
    gpds::archiver_xml ar;
    std::stringstream stream(FILE_CONTENT);
    std::unique_ptr<gpds::document> doc = ar.load(stream);
    REQUIRE(doc);

    SUBCASE("Query a list of elements") {
        std::vector<std::string> values = doc->query_list("/data/boolean");
        REQUIRE(values.size() == 2);
        REQUIRE(values.front() == "true");
        REQUIRE(values.back() == "false");
    }

    SUBCASE("Find all the nodes with a certain name") {
        std::vector<std::string> values = doc->query_list("//boolean");
        REQUIRE(values.size() == 2);
        REQUIRE(values.front() == "true");
        REQUIRE(values.back() == "false");
    }

    SUBCASE("Find all the nodes with a certain name and attribute") {
        std::vector<std::string> values = doc->query_list("//boolean[@id='1234']");
        REQUIRE(values.size() == 1);
        REQUIRE(values.front() == "false");
    }

    SUBCASE("Find all the nodes with a certain name and attribute") {
        std::vector<gpds::value> values = doc->query_values("data/boolean");
        REQUIRE(values.size() == 2);
        REQUIRE(values.front().get<bool>() == true);
        REQUIRE(values.back().get<bool>() == false);
    }

    SUBCASE("Find all the nodes with a certain name as fragments") {
        std::vector<std::unique_ptr<gpds::fragment>> values = doc->query_fragments("/data/boolean");
        REQUIRE(values.size() == 2);
    }
}

TEST_CASE("Convert document to string") {
    gpds::archiver_xml ar;
    std::stringstream stream(FILE_CONTENT);
    std::unique_ptr<gpds::document> doc = ar.load(stream);
    REQUIRE(doc);

    REQUIRE(doc->to_string() == FILE_CONTENT);
}

TEST_CASE("Copy the document") {
    gpds::archiver_xml ar;
    std::stringstream stream(FILE_CONTENT);
    std::unique_ptr<gpds::document> doc = ar.load(stream);
    REQUIRE(doc);

    gpds::document_xml newDoc = *dynamic_cast<gpds::document_xml*>(doc.get());
    REQUIRE(newDoc.query("/data/boolean") == "true");
}

#endif
