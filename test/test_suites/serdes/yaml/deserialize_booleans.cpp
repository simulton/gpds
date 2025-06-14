#include "../../test.hpp"

#include <gpds/archiver_yaml.hpp>

#include <vector>
#include <sstream>

static const std::string FILE_CONTENT = R"""(
---
data:
  boolean: 
    - true
    - false
)""";

class test_data_25 : public gpds::serialize
{
public:
    std::vector<bool> data;

    [[nodiscard]]
    gpds::container
    to_container() const override
    {
        return {};
    }

    void
    from_container(const gpds::container& object) override
    {
        data = object.get_values<bool>("boolean");
    }
};

TEST_SUITE("serdes - yaml")
{

    TEST_CASE("Read Datatype: Boolean")
    {
        // The "known good" data
        const std::vector<bool> knownGood = {
            true,
            false
        };

        // Parse test file
        test_data_25 data;
        gpds_test::deserialize<gpds::archiver_yaml>(FILE_CONTENT, data, "data");

        // Ensure that data is the same
        CHECK_EQ(data.data, knownGood);
    }

}
