#pragma once

#include <string>
#include <ostream>
#include <istream>

namespace Gpds {
    class Serialize;
}

namespace GpdsTest {
    class Test {
    public:
        static bool serialize(std::ostream &stream, Gpds::Serialize &object, const std::string &rootName);
        static bool deserialize(std::istream &stream, Gpds::Serialize &object, const std::string &rootName);
    };
}
