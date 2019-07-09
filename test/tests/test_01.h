#include <vector>
#include <iostream>
#include "../3rdparty/catch2/catch.hpp"
#include "../test.h"
#include "gpds.h"

class TestData01 : public Gpds::Serialize
{
public:
    std::vector<bool> data;

    virtual Gpds::Container toContainer() const override
    {
        return { };
    }

    virtual void fromContainer( const Gpds::Container& object ) override
    {
        data = object.getValues<bool>( "boolean" );
    }
};

TEST_CASE( "Test 01 - Datatype: Boolean" )
{
    // The "known good" data
    const std::vector<bool> knownGood = {
        true,
        false
    };

    // Parse test file
    TestData01 data;
    REQUIRE( read_file( "../test/data/test_data_01.xml", data ) );

    // Ensure that data is the same
    REQUIRE( data.data == knownGood );
}

