#include "../../test.hpp"

#include "gpds/archiver_yaml.hpp"

static constexpr const char* brand = "Ferrari";

TEST_SUITE("serdes - yaml")
{

    TEST_CASE("Serialize and deserialize a container to and from a file")
    {
        // Create a container
        gpds::container container;
        container.add_value("brand", brand);

        // Setup
        gpds::archiver_yaml ar;
        std::filesystem::path path("test_38.yaml");

        // Make sure the file doesn't exist
        if (std::filesystem::exists(path))
            REQUIRE_MESSAGE(std::filesystem::remove(path), "could not remove file");

        // Save
        auto ret = gpds::to_file<gpds::archiver_yaml>(path, container, "data");
        REQUIRE_MESSAGE(ret.first, ret.second);

        // Load
        gpds::container newContainer;
        ret = gpds::from_file<gpds::archiver_yaml>(path, newContainer, "data");
        REQUIRE_MESSAGE(ret.first, ret.second);

        // Check that the value is correct
        auto brandOpt = newContainer.get_value<std::string>("brand");
        CHECK(brandOpt.has_value());
        CHECK(brandOpt.value() == brand);

        // Clean up
        std::filesystem::remove(path);
    }

    TEST_CASE("Deserializing from an inexistent file returns false")
    {
        // Setup
        gpds::archiver_yaml ar;
        std::filesystem::path path("test_38_inexistent.yaml");

        // Make sure the file doesn't exist
        if (std::filesystem::exists(path))
            REQUIRE_MESSAGE(std::filesystem::remove(path), "could not remove file");

        // Load and check that it returns false
        gpds::container container;
        const auto ret = gpds::from_file<gpds::archiver_yaml>(path, container, "data");
        REQUIRE_FALSE_MESSAGE(ret.first, ret.second);
    }

    TEST_CASE("Serializing to an invalid file path")
    {
        // Setup
        gpds::archiver_yaml ar;
        std::filesystem::path path("/");

        // Load and make sure it returns false
        gpds::container container;
        container.add_attribute("height", "123px");
        const auto ret = gpds::to_file<gpds::archiver_yaml>(path, container, "div");
        REQUIRE_FALSE_MESSAGE(ret.first, ret.second);
    }

}
