#include <sstream>
#include "gpds/serialize.hpp"
#include "../test.h"

class list_order : public gpds::serialize
{
public:
    std::vector<int> data;

    virtual gpds::container to_container() const override
    {
        gpds::container root;
        for (int i: data) {
            root.add_value("value", i);
        }
        return root;
    }

    virtual void from_container(const gpds::container& object) override
    {
        std::vector<int> tmp;
        for (auto& value: object.get_values<int>("value")) {
            tmp.push_back(value);
        }
        data = tmp;
    }
};

TEST_CASE("List order is consistent")
{
    list_order list1;
    for (int i = 0; i < 10; i++) {
        list1.data.push_back(i);
    }

    // Serialize
    std::stringstream serialized;
    gpds_test::test::serialize(serialized, list1, "listorder");

    // Deserialize
    list_order list2;
    gpds_test::test::deserialize(serialized, list2, "listorder");

    // Ensure that data is the same
    REQUIRE(list1.data == list2.data);
}

