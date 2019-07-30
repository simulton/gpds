#include "test.h"

#include <iostream>
#include "serialize.h"
#include "archiverxml.h"

bool GpdsTest::Test::serialize( std::ostream& stream, Gpds::Serialize& object, const std::string& rootName )
{
    Gpds::ArchiverXml ar;

    bool ret = false;
    try {
        ret = ar.save(stream, object, rootName);
    } catch ( std::exception& e ) {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
        return false;
    }

    return ret;
}

bool GpdsTest::Test::deserialize( std::istream& stream, Gpds::Serialize& object, const std::string& rootName )
{
    Gpds::ArchiverXml ar;

    bool ret = false;
    try {
        ret = ar.load(stream, object, rootName);
    } catch ( std::exception& e ) {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
        return false;
    }

    return ret;
}
