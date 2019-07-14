#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include "gpds/serialize.h"
#include "gpds/archiverxml.h"

static bool deserialize( const std::string& content , Gpds::Serialize& object )
{
    std::stringstream stream( content );

    Gpds::ArchiverXml ar;

    bool ret = false;
    try {
        ret = ar.load(stream, object, "data");
    } catch ( std::exception& e ) {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
        return false;
    }

    return ret;
}
