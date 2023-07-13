#include "test.hpp"

#include "gpds/serialize.hpp"
#include "gpds/archiver_xml.hpp"
#include "gpds/archiver_yaml.hpp"

#include <sstream>

void gpds_test::test::serialize(std::ostream& stream, gpds::serialize& object, const std::string& rootName, enum gpds::serialize::mode mode)
{
    bool ret = false;
    REQUIRE((mode == gpds::serialize::mode::XML || mode == gpds::serialize::mode::YAML));
    if (mode == gpds::serialize::mode::XML) {
        gpds::archiver_xml ar;
        REQUIRE_NOTHROW(ret = ar.save(stream, object, rootName));
    } else if (mode == gpds::serialize::mode::YAML) {
        gpds::archiver_yaml ar;
        REQUIRE_NOTHROW(ret = ar.save(stream, object, rootName));
    }
    REQUIRE(ret);
}

void gpds_test::test::deserialize(std::istream& stream, gpds::serialize& object, const std::string& root_name, enum gpds::serialize::mode mode)
{
    bool ret = false;
    REQUIRE((mode == gpds::serialize::mode::XML || mode == gpds::serialize::mode::YAML));
    if (mode == gpds::serialize::mode::XML) {
        gpds::archiver_xml ar;
        REQUIRE_NOTHROW(ret = ar.load(stream, object, root_name));
    } else if (mode == gpds::serialize::mode::YAML) {
        gpds::archiver_yaml ar;
        REQUIRE_NOTHROW(ret = ar.load(stream, object, root_name));
    }
    REQUIRE(ret);
}

void gpds_test::test::deserialize(const std::string& str, gpds::serialize& object, const std::string& root_name, enum gpds::serialize::mode mode)
{
    std::stringstream stream(str);
    return deserialize(stream, object, root_name, mode);
}
