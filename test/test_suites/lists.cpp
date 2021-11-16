#include "../test.hpp"

class list_order :
    public gpds::serialize
{
public:
    std::vector<int> data;

    virtual gpds::container to_container() const override
    {
        gpds::container root;
        for (int i: data)
            root.add_value("value", i);
        return root;
    }

    virtual void from_container(const gpds::container& object) override
    {
        std::vector<int> tmp;
        for (auto& value: object.get_values<int>("value"))
            tmp.push_back(value);
        data = tmp;
    }
};

TEST_SUITE("lists")
{

    TEST_CASE("container can contain a list of elements")
    {
        gpds::container container;

        SUBCASE("creating list container")
        {
            static constexpr std::size_t list_size = 10;

            CHECK_FALSE(container.is_list());

            for (int i = 0; i < list_size; i++)
                container.add_value("list-item", i);

            CHECK(container.is_list());

            std::vector<int> list = container.get_values<int>("list-item");
            CHECK_EQ(list.size(), list_size);
        }
    }

    TEST_CASE("list order is consistent")
    {
        list_order list1;
        for (int i = 0; i < 10; i++)
            list1.data.push_back(i);

        // Serialize
        std::stringstream serialized;
        gpds_test::test::serialize(serialized, list1, "listorder");

        // Deserialize
        list_order list2;
        gpds_test::test::deserialize(serialized, list2, "listorder");

        // Ensure that data is the same
        CHECK_EQ(list1.data, list2.data);
    }

}