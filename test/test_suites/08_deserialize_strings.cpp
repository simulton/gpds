#include <vector>
#include <sstream>
#include "../test.hpp"

static const std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<data>"
    "  <string>Hello World!</string>"
    "  <string>2019-08-09</string>"
    "  <string>1234test</string>"
    "  <string>1.2test</string>"
    "  <string>test1234</string>"
    "  <string>test1.2</string>"
    "  <string>1 + x = 5</string>"
    "  <string>1970-01-01</string>"
    "  <string>04.02.2001</string>"
    "  <string>12/5/18</string>"
    "  <string>2018-1-13</string>"
    "  <string>a</string>"
    "  <string>_under_score_</string>"
    "  <string>+-={}[]/\\!@#$%^&*()_</string>"
    "  <string>\\n\\r\\b</string>"
    "</data>";

class test_data_8 : public gpds::serialize
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
    test_data_8 data;
    gpds_test::test::deserialize(FILE_CONTENT, data, "data");

    // Ensure that data is the same
    REQUIRE(data.data == knownGood);
}

