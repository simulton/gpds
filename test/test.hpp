#pragma once

#include <string>
#include <istream>
#include <ostream>
#include <sstream>

#include "doctest.hpp"

#include "gpds/serialize.hpp"

namespace gpds
{
    class serialize;
}

namespace gpds_test
{

    template<typename Archiver>
    static
    void
    serialize(std::ostream& stream, gpds::serialize& object, const std::string& root_name)
    {
        const auto ret = gpds::to_stream<Archiver>(stream, object, root_name);      // ToDo: Wrap this in REQUIRE_NOTHROW()
        REQUIRE_MESSAGE(ret, ret.error().message());
    }

    template<typename Archiver>
    static
    void
    deserialize(std::istream& stream, gpds::serialize& object, const std::string& root_name)
    {
        const auto ret = gpds::from_stream<Archiver>(stream, object, root_name);      // ToDo: Wrap in REQUIRE_NOTHROW()
        REQUIRE_MESSAGE(ret, ret.error().message());
    }

    template<typename Archiver>
    static
    void
    deserialize(const std::string& str, gpds::serialize& object, const std::string& root_name)
    {
        std::stringstream stream(str);
        return deserialize<Archiver>(stream, object, root_name);
    }

}
