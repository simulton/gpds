#pragma once

#include <string>
#include <ostream>
#include <istream>

#include "doctest.hpp"

#include "gpds/serialize.hpp"
#include "gpds/archiver_xml.hpp"
#include "gpds/archiver_yaml.hpp"

namespace gpds
{
    class serialize;
}

namespace gpds_test
{
    class test
    {
    public:
        static void serialize(std::ostream& stream, gpds::serialize& object, const std::string& rootName, enum gpds::serialize::mode mode = gpds::serialize::mode::XML);
        static void deserialize(std::istream& stream, gpds::serialize& object, const std::string& rootName, enum gpds::serialize::mode mode = gpds::serialize::mode::XML);
        static void deserialize(const std::string& str, gpds::serialize& object, const std::string& rootName, enum gpds::serialize::mode mode = gpds::serialize::mode::XML);
    };
}
