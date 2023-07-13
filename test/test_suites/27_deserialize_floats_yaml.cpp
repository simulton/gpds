#include "../test.hpp"

#include <vector>

static const std::string FILE_CONTENT =
R"""(
---
data: 
  real: 
    - "-0."
    - 0.
    - "-1."
    - 1.
)""";

class test_data_27 : public gpds::serialize
{
public:
    std::vector<double> data;

    virtual gpds::container to_container() const override
    {
        return {};
    }

    virtual void from_container(const gpds::container& object) override
    {
        data = object.get_values<double>("real");
    }
};

TEST_CASE("Read Datatype: Real")
{
    // The "known good" data
    const std::vector<double> knownGood = {
        0,
        0,
        -1,
        1
    };

    // Parse test file
    test_data_27 data;
    gpds_test::deserialize<gpds::archiver_yaml>(FILE_CONTENT, data, "data");

    // Ensure that data is the same
    CHECK_EQ(data.data, knownGood);
}
