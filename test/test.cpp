#include "test.h"

#include <iostream>
#include "serialize.hpp"
#include "archiver_xml.hpp"

bool gpds_test::test::serialize(std::ostream& stream, gpds::serialize& object, const std::string& rootName)
{
    gpds::archiver_xml ar;

    bool ret = false;
    try {
        ret = ar.save(stream, object, rootName);
    } catch (std::exception& e) {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
        return false;
    }

    return ret;
}

bool gpds_test::test::deserialize(std::istream& stream, gpds::serialize& object, const std::string& rootName)
{
    gpds::archiver_xml ar;

    bool ret = false;
    try {
        ret = ar.load(stream, object, rootName);
    } catch (std::exception& e) {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
        return false;
    }

    return ret;
}
