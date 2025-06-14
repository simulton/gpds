#include "../../test.hpp"

#include <gpds/archiver_xml.hpp>

#include <vector>

static const std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<data>"
    "  <int>-0</int>"
    "  <int>0</int>"
    "  <int>-1</int>"
    "  <int>1</int>"
    "  <int>-42</int>"
    "  <int>42</int>"
    "  <int>-1024</int>"
    "  <int>1023</int>"
    "  <int>-32768</int>"
    "  <int>32767</int>"
    "</data>";

class test_data_6 : public gpds::serialize
{
public:
    std::vector<int> data;

    [[nodiscard]]
    gpds::container
    to_container() const override
    {
        return {};
    }

    void
    from_container(const gpds::container& object) override
    {
        data = object.get_values<int>("int");
    }
};

TEST_SUITE("serdes - xml")
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
        test_data_6 data;
        gpds_test::deserialize<gpds::archiver_xml>(FILE_CONTENT, data, "data");

        // Ensure that data is the same
        CHECK_EQ(data.data, knownGood);
    }

}
