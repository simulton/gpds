#include <vector>
#include "../3rdparty/catch2/catch.hpp"
#include "../test.h"
#include "gpds.h"

class TestData03 : public Gpds::Serialize
{
public:
    std::vector<double> data;

    virtual Gpds::Container toContainer() const override
    {
        return { };
    }

    virtual void fromContainer( const Gpds::Container& object ) override
    {
        data = object.getValues<double>( "real" );
    }
};

TEST_CASE( "Test 03 - Datatype: Real" )
{
    // The "known good" data
    const std::vector<double> knownGood = {
        0,
        0,
        -1,
        1
    };

    // Parse test file
    TestData03 data;
    REQUIRE( read_file( "../test/data/test_data_03.xml", data ) );

    // Ensure that data is the same
    REQUIRE( data.data == knownGood );
}

