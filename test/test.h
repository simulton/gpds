#pragma once

#include <optional>
#include <string>
#include <iostream>
#include "gpds.h"

static bool read_file( const std::string& path, Gpds::Serialize& object )
{
    std::ifstream file( path );
    if ( not file.is_open() ) {
        return false;
    }

    Gpds::ArchiverXml ar;

    bool ret = false;
    try {
        ret = ar.load(file, object, "data");
    } catch ( std::exception& e ) {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
        return false;
    }

    return ret;
}
