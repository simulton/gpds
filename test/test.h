#pragma once

#include <string>
#include <ostream>
#include <istream>

#include "doctest.hpp"

namespace gpds
{
    class serialize;
}

namespace gpds_test
{
    class test
    {
    public:
        static bool serialize(std::ostream& stream, gpds::serialize& object, const std::string& rootName);
        static bool deserialize(std::istream& stream, gpds::serialize& object, const std::string& rootName);
    };
}
