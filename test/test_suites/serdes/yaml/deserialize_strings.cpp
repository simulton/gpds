#include <vector>
#include <sstream>
#include "../../test.hpp"

static const std::string FILE_CONTENT =
R"""(
---
data: 
  string: 
    - "Hello World!"
    - "2019-08-09"
    - 1234test
    - 1.2test
    - test1234
    - test1.2
    - "1 + x = 5"
    - "1970-01-01"
    - 04.02.2001
    - 12/5/18
    - "2018-1-13"
    - a
    - _under_score_
    - "+-={}[]/\!@#$%^&*()_"
    - \n\r\b
)""";

class test_data_28 : public gpds::serialize
{
public:
    std::vector<std::string> data;

    virtual gpds::container to_container() const override
    {
        return {};
    }

    virtual void from_container(const gpds::container& object) override
    {
        data = object.get_values<std::string>("string");
    }
};

TEST_SUITE("serdes - yaml")
{

    TEST_CASE("Read Datatype: String")
    {
        // The "known good" data
        const std::vector<std::string> knownGood = {
            "Hello World!",
            "2019-08-09",
            "1234test",
            "1.2test",
            "test1234",
            "test1.2",
            "1 + x = 5",
            "1970-01-01",
            "04.02.2001",
            "12/5/18",
            "2018-1-13",
            "a",
            "_under_score_",
            "+-={}[]/\\!@#$%^&*()_",
            "\\n\\r\\b"
        };

        // Parse test file
        test_data_28 data;
        gpds_test::deserialize<gpds::archiver_yaml>(FILE_CONTENT, data, "data");

        // Ensure that data is the same
        CHECK_EQ(data.data, knownGood);
    }

}
