#include <sstream>
#include "doctest.h"
#include "serialize.h"
#include "../test.h"

class ListOrder : public Gpds::Serialize
{
public:
    std::vector<int> data;

    virtual Gpds::Container toContainer() const override
    {
        Gpds::Container root;
        for (int i: data){
            root.addValue("value", i);
        }
        return root;
    }

    virtual void fromContainer( const Gpds::Container& object ) override
    {
        std::vector<int> tmp;
        for (auto& value: object.getValues<int>("value")) {
            tmp.push_back(value);
        }
        data = tmp;
    }
};

TEST_CASE( "List order is consistent" )
{
    ListOrder list1;
    for (int i = 0; i < 10; i++) {
        list1.data.push_back(i);
    }

    // Serialize
    std::stringstream serialized;
    REQUIRE( GpdsTest::Test::serialize( serialized, list1, "listorder" ) );

    // Deserialize
    ListOrder list2;
    REQUIRE( GpdsTest::Test::deserialize( serialized, list2, "listorder" ) );

    // Ensure that data is the same
    REQUIRE( list1.data == list2.data );
}

