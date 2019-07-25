#include <vector>
#include "catch2/catch.hpp"
#include "../test.h"
#include "gpds/serialize.h"

static std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<data>"
    "  <string>Hello World!</string>"
    "</data>";

class TestData04 : public Gpds::Serialize
{
public:
    std::vector<std::string> data;

    virtual Gpds::Container toContainer() const override
    {
        return { };
    }

    virtual void fromContainer( const Gpds::Container& object ) override
    {
        data = object.getValues<std::string>( "string" );
    }
};

TEST_CASE( "Read Datatype: String" )
{
    // The "known good" data
    const std::vector<std::string> knownGood = {
        "Hello World!"
    };

    // Parse test file
    TestData04 data;
    REQUIRE( deserialize( FILE_CONTENT, data, "data" ) );

    // Ensure that data is the same
    REQUIRE( data.data == knownGood );
}

