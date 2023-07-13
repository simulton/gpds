#pragma once

#include <string>
#include <istream>
#include <ostream>
#include <sstream>

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

    template<typename Archiver = gpds::archiver_xml>
    static
    void
    serialize(std::ostream& stream, gpds::serialize& object, const std::string& root_name)
    {
        bool ret = false;

        Archiver ar;
        REQUIRE_NOTHROW(ret = ar.save(stream, object, root_name));

        REQUIRE(ret);
    }

    template<typename Archiver = gpds::archiver_xml>
    static
    void
    deserialize(std::istream& stream, gpds::serialize& object, const std::string& root_name)
    {
        bool ret = false;

        Archiver ar;
        REQUIRE_NOTHROW(ret = ar.load(stream, object, root_name));

        REQUIRE(ret);
    }

    template<typename Archiver = gpds::archiver_xml>
    static
    void
    deserialize(const std::string& str, gpds::serialize& object, const std::string& root_name)
    {
        std::stringstream stream(str);
        return deserialize<Archiver>(stream, object, root_name);
    }

}
