#include "../test.hpp"

#include <vector>
#include <sstream>

static const std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<data>"
    "  <value attribute=\"true\">5</value>"
    "  <value attribute=\"false\">0</value>"
    "  <value attribute=\"0\"></value>"
    "  <value attribute=\"1\">lorem</value>"
    "  <value attribute=\"42\"></value>"
    "  <value attribute=\"-17\">1234</value>"
    "  <value attribute=\"-0.00\"></value>"
    "  <value attribute=\"0.00\">-1234</value>"
    "  <value attribute=\"-1.00\"></value>"
    "  <value attribute=\"1.00\">2.3</value>"
    "  <value attribute=\"Hello GPDS!\"></value>"
    "  <value attribute=\"Hello GPDS!\"/>"
    "</data>";

using data = std::vector<std::variant<
    bool,
    int,
    double,
    std::string
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
    std::string("Hello GPDS!"),
    std::string("Hello GPDS!"),
};

class test_data_9 : public gpds::serialize
{
public:
    data d;

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
                        d.emplace_back(it->second.get_attribute<bool>("attribute").value());
                        break;

                    case 2:
                    case 3:
                    case 4:
                    case 5:
                        d.emplace_back(it->second.get_attribute<int>("attribute").value());
                        break;

                    case 6:
                    case 7:
                    case 8:
                    case 9:
                        d.emplace_back(it->second.get_attribute<double>("attribute").value());
                        break;

                    case 10:
                    case 11:
                        d.emplace_back(it->second.get_attribute<std::string>("attribute").value());
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
    gpds_test::test::deserialize(FILE_CONTENT, testData, "data");

    // Ensure that data is the same
    REQUIRE(testData.d.size() == knownGood.size());
    REQUIRE(testData.d == knownGood);
}

