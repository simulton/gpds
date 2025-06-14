#include "../../test.hpp"

#include <gpds/archiver_yaml.hpp>

#include <vector>
#include <sstream>

static const std::string FILE_CONTENT =
R"""(
---
data: 
  value: 
    - "-attribute": true
    - "-attribute": false
    - "-attribute": 0
    - "-attribute": 1
    - "-attribute": 42
    - "-attribute": "-17"
    - "-attribute": "-0"
    - "-attribute": 0
    - "-attribute": "-1"
    - "-attribute": 1
    - "-attribute": "Hello GPDS!"
)""";

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

class test_data_20 : public gpds::serialize
{
public:
    data d;

    [[nodiscard]]
    gpds::container
    to_container() const override
    {
        return {};
    }

    void
    from_container(const gpds::container& object) override
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

TEST_SUITE("serdes - yaml")
{

    TEST_CASE("Read Attributes: Container Attributes")
    {
        // Parse test file
        test_data_20 testData;
        gpds_test::deserialize<gpds::archiver_yaml>(FILE_CONTENT, testData, "data");

        // Ensure that data is the same
        CHECK_EQ(testData.d.size(), knownGood.size());
        CHECK_EQ(testData.d, knownGood);
    }

}
