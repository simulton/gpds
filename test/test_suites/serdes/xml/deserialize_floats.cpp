#include "../../test.hpp"

#include <gpds/archiver_xml.hpp>

#include <vector>

static const std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<data>"
    "  <real>-0.0</real>"
    "  <real>0.0</real>"
    "  <real>-1.0</real>"
    "  <real>1.0</real>"
    "</data>";

class test_data_7 : public gpds::serialize
{
public:
    std::vector<double> data;

    [[nodiscard]]
    gpds::container
    to_container() const override
    {
        return {};
    }

    void
    from_container(const gpds::container& object) override
    {
        data = object.get_values<double>("real");
    }
};

TEST_SUITE("serdes - xml")
{

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
        test_data_7 data;
        gpds_test::deserialize<gpds::archiver_xml>(FILE_CONTENT, data, "data");

        // Ensure that data is the same
        CHECK_EQ(data.data, knownGood);
    }

}
