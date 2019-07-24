#include <vector>
#include "catch2/catch.hpp"
#include "../test.h"
#include "gpds/serialize.h"

static const std::string FILE_CONTENT =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<data>"
    "  <value attribute=\"true\">0</value>"
    "  <value attribute=\"false\">0</value>"
    "  <value attribute=\"0\">0</value>"
    "  <value attribute=\"1\">0</value>"
    "  <value attribute=\"42\">0</value>"
    "  <value attribute=\"-17\">0</value>"
    "  <value attribute=\"-0.00\">0</value>"
    "  <value attribute=\"0.00\">0</value>"
    "  <value attribute=\"-1.00\">0</value>"
    "  <value attribute=\"1.00\">0</value>"
    "  <value attribute=\"Hello GPDS!\">0</value>"
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

class TestData05 : public Gpds::Serialize
{
public:
    Data data;

    virtual Gpds::Container toContainer() const override
    {
        return { };
    }

    virtual void fromContainer( const Gpds::Container& object ) override
    {
        // Get all values
        for ( auto it = object.values.cbegin(); it != object.values.cend(); it++ ) {
            if ( it->first.compare( "value" ) == 0 ) {
                // Figure out which type we need
                std::size_t index = std::distance( object.values.cbegin(), it );
                switch ( index ) {
                    case 0:
                    case 1:
                        data.emplace_back( it->second.getAttribute<Gpds::gBool>( "attribute" ).value( ) );
                        break;

                    case 2:
                    case 3:
                    case 4:
                    case 5:
                        data.emplace_back( it->second.getAttribute<Gpds::gInt>( "attribute" ).value(  ) );
                        break;

                    case 6:
                    case 7:
                    case 8:
                    case 9:
                        data.emplace_back( it->second.getAttribute<Gpds::gReal>( "attribute" ).value( ) );
                        break;

                    case 10:
                        data.emplace_back( it->second.getAttribute<Gpds::gString>( "attribute" ).value( ) );
                        break;
                }

            }
        }
        REQUIRE( data.size() == knownGood.size() );
    }
};

TEST_CASE( "Read Attributes: Value Attributes" )
{
    // Parse test file
    TestData05 testData;
    REQUIRE( deserialize( FILE_CONTENT, testData ) );

    // Ensure that data is the same
    REQUIRE( testData.data.size() == knownGood.size() );
    REQUIRE( testData.data == knownGood );
}

