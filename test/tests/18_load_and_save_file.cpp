#include <iostream>
#include "doctest.h"
#include "gpds/archiver_xml.hpp"

#define BRAND "Ferrari"

TEST_CASE("Serialize and deserialize a container to and from a file")
{
    // Create a container
    gpds::container container;
    container.add_value("brand", BRAND);

    // Setup
    gpds::archiver_xml ar;
    std::filesystem::path path("test_18.xml");

    // Make sure the file doesn't exist
    if (std::filesystem::exists(path)) {
        REQUIRE_MESSAGE(std::filesystem::remove(path),
          "Could not remove file " + std::filesystem::absolute(path).string());
    }

    // Save
    REQUIRE(ar.save(path, container, "data"));

    // Load
    gpds::container newContainer;
    REQUIRE(ar.load(path, newContainer, "data"));

    // Check that the value is correct
    auto brandOpt = newContainer.get_value<std::string>("brand");
    REQUIRE(brandOpt.has_value());
    REQUIRE(brandOpt.value() == BRAND);

    // Clean up
    std::filesystem::remove(path);
}

TEST_CASE("Deserializing from an inexistent file returns false") {
    // Setup
    gpds::archiver_xml ar;
    std::filesystem::path path("test_18_inexistent.xml");

    // Make sure the file doesn't exist
    if (std::filesystem::exists(path)) {
        REQUIRE_MESSAGE(std::filesystem::remove(path),
          "Could not remove file " + std::filesystem::absolute(path).string());
    }

    // Load and check that it returns false
    gpds::container container;
    REQUIRE_FALSE(ar.load(path, container, "data"));
}

TEST_CASE("Serializing to an invalid file path") {
    // Setup
    gpds::archiver_xml ar;
    std::filesystem::path path("/");

    // Load and make sure it returns false
    gpds::container container;
    container.add_attribute("height", "123px");
    REQUIRE_FALSE(ar.save(path, container, "div"));
}
