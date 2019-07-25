#include <vector>
#include <sstream>
#include "catch2/catch.hpp"
#include "../test.h"
#include "gpds/serialize.h"

static const std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<data>"
    "  <boolean>true</boolean>"
    "  <boolean>false</boolean>"
    "</data>";

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

TEST_CASE( "Read Datatype: Boolean" )
{
    // The "known good" data
    const std::vector<bool> knownGood = {
        true,
        false
    };

    // Parse test file
    TestData01 data;
    std::stringstream stream( FILE_CONTENT );
    REQUIRE( GpdsTest::Test::deserialize( stream, data, "data" ) );

    // Ensure that data is the same
    REQUIRE( data.data == knownGood );
}
