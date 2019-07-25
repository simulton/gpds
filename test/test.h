#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include "gpds/serialize.h"
#include "gpds/archiverxml.h"

static bool serialize( std::string& string, const Gpds::Serialize& object, const std::string& rootName )
{
    Gpds::ArchiverXml ar;

    bool ret = false;
    try {
        ret = ar.save( string, object, rootName );
    } catch ( std::exception& e ) {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
        return false;
    }

    return ret;
}

static bool deserialize( std::string& content, Gpds::Serialize& object, const std::string& rootName )
{
    Gpds::ArchiverXml ar;

    bool ret = false;
    try {
        ret = ar.load(content, object, rootName);
    } catch ( std::exception& e ) {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
        return false;
    }

    return ret;
}
