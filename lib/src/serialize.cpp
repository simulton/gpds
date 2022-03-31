#include "serialize.hpp"
#include "archiver_xml.hpp"

using namespace gpds;

std::pair<bool, std::string>
serialize::to_string(std::string& str, const std::string& root_name) const
{
    archiver_xml ar;
    const bool& ret = ar.save(str, *this, root_name);

    return { ret, "" };
}

std::pair<bool, std::string>
serialize::from_string(const std::string_view& str, const std::string& root_name)
{
    archiver_xml ar;
    const bool& ret = ar.load(std::string{str}, *this, root_name);        // ToDo: Add std::string_view interface to archiver_xml

    return { ret, "" };
}

std::pair<bool, std::string>
serialize::to_file(const std::filesystem::path& path, const std::string& root_name) const
{
    archiver_xml ar;
    const bool& ret = ar.save(path, *this, root_name);

    return { ret, "" };
}

std::pair<bool, std::string>
serialize::from_file(const std::filesystem::path& path, const std::string& root_name)
{
    if (!std::filesystem::exists(path))
        return { false, "File does not exist." };

    archiver_xml ar;
    const bool& ret = ar.load(path, *this, root_name);

    return { ret, "" };
}
