#include <vector>
#include <sstream>
#include "doctest.h"
#include "../test.h"
#include "serialize.h"

static const std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<data>"
    "  <value attribute=\"true\">0</value>"
    "  <value attribute=\"false\">0</value>"
    "  <value attribute=\"0\">0</value>"
    "  <value attribute=\"1\">0</value>"
    "  <value attribute=\"42\">0</value>"
    "  <value attribute=\"-17\">0</value>"
    "  <value attribute=\"-0.00\">0</value>"
    "  <value attribute=\"0.00\">0</value>"
    "  <value attribute=\"-1.00\">0</value>"
    "  <value attribute=\"1.00\">0</value>"
    "  <value attribute=\"Hello GPDS!\">0</value>"
    "</data>";

using data = std::vector<std::variant<
    gpds::gBool,
    gpds::gInt,
    gpds::gReal,
    gpds::gString
>>;

// The "known good" data
static const data knownGood = {
    true,
    false,
    0,
    1,
    42,
    -17,
    -0.0,
    0.0,
    -1.0,
    1.0,
    std::string("Hello GPDS!")
};

class test_data_9 : public gpds::serialize
{
public:
    data data;

    virtual gpds::container to_container() const override
    {
        return {};
    }

    virtual void from_container(const gpds::container& object) override
    {
        // Get all values
        for (auto it = object.values.cbegin(); it != object.values.cend(); it++) {
            if (it->first.compare("value") == 0) {
                // Figure out which type we need
                std::size_t index = std::distance(object.values.cbegin(), it);
                switch (index) {
                    case 0:
                    case 1:
                        data.emplace_back(it->second.get_attribute<gpds::gBool>("attribute").value());
                        break;

                    case 2:
                    case 3:
                    case 4:
                    case 5:
                        data.emplace_back(it->second.get_attribute<gpds::gInt>("attribute").value());
                        break;

                    case 6:
                    case 7:
                    case 8:
                    case 9:
                        data.emplace_back(it->second.get_attribute<gpds::gReal>("attribute").value());
                        break;

                    case 10:
                        data.emplace_back(it->second.get_attribute<gpds::gString>("attribute").value());
                        break;
                }
            }
        }
    }
};

TEST_CASE("Read Attributes: Value Attributes")
{
    // Parse test file
    test_data_9 testData;
    std::stringstream stream(FILE_CONTENT);
    REQUIRE(gpds_test::test::deserialize(stream, testData, "data"));

    // Ensure that data is the same
    REQUIRE(testData.data.size() == knownGood.size());
    REQUIRE(testData.data == knownGood);
}

