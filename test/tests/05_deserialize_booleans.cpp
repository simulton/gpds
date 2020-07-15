#include <vector>
#include <sstream>
#include "doctest.h"
#include "../test.h"
#include "gpds/serialize.hpp"

static const std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<data>"
    "  <boolean>true</boolean>"
    "  <boolean>false</boolean>"
    "</data>";

class test_data_5 : public gpds::serialize
{
public:
    std::vector<bool> data;

    virtual gpds::container to_container() const override
    {
        return {};
    }

    virtual void from_container(const gpds::container& object) override
    {
        data = object.get_values<bool>("boolean");
    }
};

TEST_CASE("Read Datatype: Boolean")
{
    // The "known good" data
    const std::vector<bool> knownGood = {
        true,
        false
    };

    // Parse test file
    test_data_5 data;
    std::stringstream stream(FILE_CONTENT);
    REQUIRE(gpds_test::test::deserialize(stream, data, "data"));

    // Ensure that data is the same
    REQUIRE(data.data == knownGood);
}
