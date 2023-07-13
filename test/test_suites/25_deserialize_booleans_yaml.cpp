#include "../test.hpp"

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
    test_data_25 data;
    gpds_test::test::deserialize(FILE_CONTENT, data, "data", gpds::serialize::mode::YAML);

    // Ensure that data is the same
    REQUIRE(data.data == knownGood);
}
