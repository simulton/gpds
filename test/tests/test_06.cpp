#include <vector>
#include <sstream>
#include "catch2/catch.hpp"
#include "../test.h"
#include "gpds/serialize.h"

static const std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<data>"
    "  <value attribute=\"true\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"false\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"0\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"1\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"42\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"-17\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"-0.00\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"0.00\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"-1.00\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"1.00\">"
    "    <child/>"
    "  </value>"
    "  <value attribute=\"Hello GPDS!\">"
    "    <child/>"
    "  </value>"
    "</data>";

using Data = std::vector<std::variant<
    Gpds::gBool,
    Gpds::gInt,
    Gpds::gReal,
    Gpds::gString
>>;

// The "known good" data
static const Data knownGood = {
    true,
    false,
    0,
    1,
    42,
    -17,
    -0.0,
    0.0,
    -1.0,
    1.0,
    std::string("Hello GPDS!")
};

class TestData06 : public Gpds::Serialize
{
public:
    Data data;

    virtual Gpds::Container toContainer() const override
    {
        return { };
    }

    virtual void fromContainer( const Gpds::Container& object ) override
    {
        const auto& valueContainers = object.getValues<Gpds::Container*>( "value" );
        for ( std::size_t i = 0; i < valueContainers.size(); i++ ) {
            const Gpds::Container* c = valueContainers.at( i );
            REQUIRE( c );

            // Figure out which type we need
            switch ( i ) {
                case 0:
                case 1:
                    data.emplace_back( c->getAttribute<Gpds::gBool>( "attribute" ).value( ) );
                    break;

                case 2:
                case 3:
                case 4:
                case 5:
                    data.emplace_back( c->getAttribute<Gpds::gInt>( "attribute" ).value(  ) );
                    break;

                case 6:
                case 7:
                case 8:
                case 9:
                    data.emplace_back( c->getAttribute<Gpds::gReal>( "attribute" ).value( ) );
                    break;

                case 10:
                    data.emplace_back( c->getAttribute<Gpds::gString>( "attribute" ).value( ) );
                    break;
            }
        }


        for ( const Gpds::Container* valueContainer : object.getValues<Gpds::Container*>( "value" ) ) {
            REQUIRE( valueContainer );


        }
    }
};

TEST_CASE( "Read Attributes: Container Attributes" )
{
    // Parse test file
    TestData06 testData;
    std::stringstream stream( FILE_CONTENT );
    REQUIRE( GpdsTest::Test::deserialize( stream, testData, "data" ) );

    // Ensure that data is the same
    REQUIRE( testData.data.size() == knownGood.size() );
    REQUIRE( testData.data == knownGood );
}

