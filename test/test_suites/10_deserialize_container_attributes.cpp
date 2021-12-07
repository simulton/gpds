#include <vector>
#include <sstream>
#include "../test.hpp"
#include "gpds/serialize.hpp"

static const std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<data>"
    "  <value attribute=\"true\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"false\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"0\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"1\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"42\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"-17\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"-0.00\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"0.00\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"-1.00\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"1.00\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"Hello GPDS!\">"
    "    <child/>"
    "  </value>"
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
    std::string("Hello GPDS!")
};

class test_data_10 : public gpds::serialize
{
public:
    data d;

    virtual gpds::container to_container() const override
    {
        return {};
    }

    virtual void from_container(const gpds::container& object) override
    {
        const auto& valueContainers = object.get_values<gpds::container*>("value");
        for (std::size_t i = 0; i < valueContainers.size(); i++) {
            const gpds::container* c = valueContainers.at(i);
            REQUIRE(c);

            // Figure out which type we need
            switch (i) {
                case 0:
                case 1:
                    d.emplace_back(c->get_attribute<bool>("attribute").value());
                    break;

                case 2:
                case 3:
                case 4:
                case 5:
                    d.emplace_back(c->get_attribute<int>("attribute").value());
                    break;

                case 6:
                case 7:
                case 8:
                case 9:
                    d.emplace_back(c->get_attribute<double>("attribute").value());
                    break;

                case 10:
                    d.emplace_back(c->get_attribute<std::string>("attribute").value());
                    break;
            }
        }


        for (const gpds::container* valueContainer : object.get_values<gpds::container*>("value")) {
            REQUIRE(valueContainer);


        }
    }
};

TEST_CASE("Read Attributes: Container Attributes")
{
    // Parse test file
    test_data_10 testData;
    gpds_test::test::deserialize(FILE_CONTENT, testData, "data");

    // Ensure that data is the same
    CHECK_EQ(testData.d.size(), knownGood.size());
    CHECK_EQ(testData.d, knownGood);
}

