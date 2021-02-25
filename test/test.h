#pragma once

#include <string>
#include <ostream>
#include <istream>

#include "doctest.hpp"

#include "gpds/serialize.hpp"
#include "gpds/archiver_xml.hpp"

namespace gpds
{
    class serialize;
}

namespace gpds_test
{
    class test
    {
    public:
        static void serialize(std::ostream& stream, gpds::serialize& object, const std::string& rootName);
        static void deserialize(std::istream& stream, gpds::serialize& object, const std::string& rootName);
        static void deserialize(const std::string& xml_string, gpds::serialize& object, const std::string& rootName);
    };
}
