#include "../../test.hpp"

#include <gpds/archiver_xml.hpp>

static constexpr const char* brand = "Ferrari";

TEST_SUITE("serdes - xml")
{

    TEST_CASE("Serialize and deserialize a container to and from a file")
    {
        // Create a container
        gpds::container container;
        container.add_value("brand", brand);

        // Setup
        gpds::archiver_xml ar;
        std::filesystem::path path("test_18.xml");

        // Make sure the file doesn't exist
        if (std::filesystem::exists(path))
            REQUIRE_MESSAGE(std::filesystem::remove(path), "could not remove file");

        // Save
        const auto success1 = gpds::to_file<gpds::archiver_xml>(path, container, "data");
        REQUIRE_MESSAGE(success1, success1.error().message());

        // Load
        gpds::container newContainer;
        const auto success2 = gpds::from_file<gpds::archiver_xml>(path, newContainer, "data");
        REQUIRE_MESSAGE(success2, success2.error().message());

        // Check that the value is correct
        auto brandOpt = newContainer.get_value<std::string>("brand");
        CHECK(brandOpt.has_value());
        CHECK_EQ(brandOpt.value(), brand);

        // Clean up
        std::filesystem::remove(path);
    }

    TEST_CASE("Deserializing from an inexistent file returns false")
    {
        // Setup
        gpds::archiver_xml ar;
        std::filesystem::path path("test_18_inexistent.xml");

        // Make sure the file doesn't exist
        if (std::filesystem::exists(path))
            REQUIRE_MESSAGE(std::filesystem::remove(path), "could not remove file");

        // Load and check that it returns false
        gpds::container container;
        const auto ret = gpds::from_file<gpds::archiver_xml>(path, container, "data");
        CHECK_FALSE_MESSAGE(ret, ret.error().message());
    }

    TEST_CASE("Serializing to an invalid file path")
    {
        // Setup
        gpds::archiver_xml ar;
        std::filesystem::path path("/");

        // Load and make sure it returns false
        gpds::container container;
        container.add_attribute("height", "123px");
        const auto ret = gpds::to_file<gpds::archiver_xml>(path, container, "div");
        CHECK_FALSE_MESSAGE(ret, ret.error().message());
    }

}
