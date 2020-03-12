#include "serialize.hpp"
#include "archiver_xml.hpp"

using namespace gpds;

std::pair<bool, std::string> serialize::to_file(const std::filesystem::path& path, const std::string& root_name) const
{
    archiver_xml ar;
    const bool& ret = ar.save(path, *this, root_name);

    return { ret, "" };
}

std::pair<bool, std::string> serialize::from_file(const std::filesystem::path& path, const std::string& root_name)
{
    if (not std::filesystem::exists(path))
        return { false, "File does not exist." };

    archiver_xml ar;
    const bool& ret = ar.load(path, *this, root_name);

    return { ret, "" };
}
