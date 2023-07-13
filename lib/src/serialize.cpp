#include "serialize.hpp"
#include "archiver_xml.hpp"
#include "archiver_yaml.hpp"

using namespace gpds;

std::pair<bool, std::string>
serialize::to_string(std::string& str, const std::string_view root_name, enum mode mode) const
{
    bool ret = false;
    switch (mode)
    {
    case mode::XML:
        {
            archiver_xml ar;
            ret = ar.save(str, *this, root_name);
        }
        break;
    case mode::YAML:
        {
            archiver_yaml ar;
            ret = ar.save(str, *this, root_name);
        }
        break;
    default:
        return { ret, "Unsupported serialize format." };
    }
    return { ret, "" };
}

std::pair<bool, std::string>
serialize::from_string(std::string_view str, const std::string_view root_name, enum mode mode)
{
    bool ret = false;
    switch (mode)
    {
    case mode::XML:
        {
            archiver_xml ar;
            // ToDo: Add std::string_view interface to archiver_xml
            ret = ar.load(std::string{str}, *this, root_name);
        }
        break;
    case mode::YAML:
        {
            archiver_yaml ar;
            // ToDo: Add std::string_view interface to archiver_yaml
            ret = ar.load(std::string{str}, *this, root_name);
        }
        break;
    default:
        return { ret, "Unsupported serialize format." };
    }
    return { ret, "" };
}

std::pair<bool, std::string>
serialize::to_file(const std::filesystem::path& path, const std::string_view root_name, enum mode mode) const
{
    bool ret = false;
    switch (mode)
    {
    case mode::XML:
        {
            archiver_xml ar;
            ret = ar.save(path, *this, root_name);
        }
        break;
    case mode::YAML:
        {
            archiver_yaml ar;
            ret = ar.save(path, *this, root_name);
        }
        break;
    default:
        return { ret, "Unsupported serialize format." };
    }
    return { ret, "" };
}

std::pair<bool, std::string>
serialize::from_file(const std::filesystem::path& path, const std::string_view root_name, enum mode mode)
{
    if (!std::filesystem::exists(path))
        return { false, "File does not exist." };

    bool ret = false;

    switch (mode)
    {
    case mode::XML:
        {
            archiver_xml ar;
            ret = ar.load(path, *this, root_name);
        }
        break;
    case mode::YAML:
        {
            archiver_yaml ar;
            ret = ar.load(path, *this, root_name);
        }
        break;
    default:
        return { ret, "Unsupported serialize format." };
    }

    return { ret, "" };
}
