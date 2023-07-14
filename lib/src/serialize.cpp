#include "serialize.hpp"
#include "archiver_xml.hpp"
#include "archiver_yaml.hpp"

using namespace gpds;

std::pair<bool, std::string>
serialize::to_string(std::string& str, const std::string_view root_name, enum mode mode) const
{
    switch (mode)
    {
        case mode::XML:     return gpds::to_string<gpds::archiver_xml>(str, *this, root_name);
        case mode::YAML:    return gpds::to_string<gpds::archiver_yaml>(str, *this, root_name);
        default:            return { false, "unsupported serialization format." };
    }
}

std::pair<bool, std::string>
serialize::from_string(std::string_view str, const std::string_view root_name, enum mode mode)
{
    switch (mode)
    {
        case mode::XML:     return gpds::from_string<gpds::archiver_xml>(str, *this, root_name);
        case mode::YAML:    return gpds::from_string<gpds::archiver_yaml>(str, *this, root_name);
        default:            return { false, "unsupported serialization format." };
    }
}

std::pair<bool, std::string>
serialize::to_file(const std::filesystem::path& path, const std::string_view root_name, enum mode mode) const
{
    switch (mode)
    {
        case mode::XML:     return gpds::to_file<gpds::archiver_xml>(path, *this, root_name);
        case mode::YAML:    return gpds::to_file<gpds::archiver_yaml>(path, *this, root_name);
        default:            return { false, "unsupported serialization format." };
    }
}

std::pair<bool, std::string>
serialize::from_file(const std::filesystem::path& path, const std::string_view root_name, enum mode mode)
{
    if (!std::filesystem::exists(path))
        return { false, "File does not exist." };

    switch (mode)
    {
        case mode::XML:     return gpds::from_file<gpds::archiver_xml>(path, *this, root_name);
        case mode::YAML:    return gpds::from_file<gpds::archiver_yaml>(path, *this, root_name);
        default:            return { false, "unsupported serialization format." };
    }
}
