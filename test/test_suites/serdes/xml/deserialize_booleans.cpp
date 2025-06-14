#include "../../test.hpp"

#include <gpds/archiver_xml.hpp>

#include <vector>
#include <sstream>

static const std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<data>"
    "  <boolean>true</boolean>"
    "  <boolean>false</boolean>"
    "</data>";

class test_data_5 : public gpds::serialize
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

TEST_SUITE("serdes - xml")
{

    TEST_CASE("Read Datatype: Boolean")
    {
        // The "known good" data
        const std::vector<bool> knownGood = {
            true,
            false
        };

        // Parse test file
        test_data_5 data;
        gpds_test::deserialize<gpds::archiver_xml>(FILE_CONTENT, data, "data");

        // Ensure that data is the same
        CHECK_EQ(data.data, knownGood);
    }

}
