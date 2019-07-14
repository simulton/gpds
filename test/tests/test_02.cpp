#include <vector>
#include <iostream>
#include "catch2/catch.hpp"
#include "../test.h"
#include "gpds/serialize.h"

class TestData02 : public Gpds::Serialize
{
public:
    std::vector<int> data;

    virtual Gpds::Container toContainer() const override
    {
        return { };
    }

    virtual void fromContainer( const Gpds::Container& object ) override
    {
        data = object.getValues<int>( "int" );
    }
};

TEST_CASE( "Read Datatype: Integer" )
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
    TestData02 data;
    REQUIRE( read_file( "../../test/data/test_data_02.xml", data ) );

    // Ensure that data is the same
    REQUIRE( data.data == knownGood );
}

