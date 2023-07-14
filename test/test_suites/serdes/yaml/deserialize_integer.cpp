#include "../../test.hpp"

#include <vector>

static const std::string FILE_CONTENT =
R"""(
---
data:
  int: 
    - -0
    - 0
    - -1
    - 1
    - -42
    - 42
    - -1024
    - 1023
    - -32768
    - 32767
)""";

class test_data_26 : public gpds::serialize
{
public:
    std::vector<int> data;

    virtual gpds::container to_container() const override
    {
        return {};
    }

    virtual void from_container(const gpds::container& object) override
    {
        data = object.get_values<int>("int");
    }
};

TEST_SUITE("serdes - yaml")
{

    TEST_CASE("Read Datatype: Integer")
    {
        // The "known good" data
        const std::vector<int> knownGood = {
            -0,
            0,
            -1,
            1,
            -42,
            42,
            -1024,
            1023,
            -32768,
            32767
        };

        // Parse test file
        test_data_26 data;
        gpds_test::deserialize<gpds::archiver_yaml>(FILE_CONTENT, data, "data");

        // Ensure that data is the same
        CHECK_EQ(data.data, knownGood);
    }

}
