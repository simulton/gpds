#include "test.hpp"

#include "gpds/serialize.hpp"
#include "gpds/archiver_xml.hpp"

#include <sstream>

void gpds_test::test::serialize(std::ostream& stream, gpds::serialize& object, const std::string& rootName)
{
    gpds::archiver_xml ar;

    bool ret = false;
    REQUIRE_NOTHROW(ret = ar.save(stream, object, rootName));
    REQUIRE(ret);
}

void gpds_test::test::deserialize(std::istream& stream, gpds::serialize& object, const std::string& root_name)
{
    gpds::archiver_xml ar;
    bool ret = false;
    REQUIRE_NOTHROW(ret = ar.load(stream, object, root_name));
    REQUIRE(ret);
}

void gpds_test::test::deserialize(const std::string& xml_str, gpds::serialize& object, const std::string& root_name)
{
    std::stringstream stream(xml_str);
    return deserialize(stream, object, root_name);
}
