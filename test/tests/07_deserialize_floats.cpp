#include <vector>
#include <sstream>
#include "doctest.h"
#include "../test.h"
#include "serialize.h"

static const std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<data>"
    "  <real>-0.0</real>"
    "  <real>0.0</real>"
    "  <real>-1.0</real>"
    "  <real>1.0</real>"
    "</data>";

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

TEST_CASE( "Read Datatype: Real" )
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
    std::stringstream stream( FILE_CONTENT );
    REQUIRE( GpdsTest::Test::deserialize( stream, data, "data" ) );

    // Ensure that data is the same
    REQUIRE( data.data == knownGood );
}
