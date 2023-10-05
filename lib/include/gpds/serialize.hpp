#pragma once

#include "container.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace gpds
{

    /**
     * An interface for (de)serializable objects.
     */
    class serialize
    {
    public:
        virtual
        ~serialize() = default;

        // Container
        [[nodiscard]] virtual gpds::container to_container() const = 0;
        virtual void from_container(const gpds::container& container) = 0;
    };

    template<typename Archiver, typename Object>
    static
    std::pair<bool, std::string>
    to_stream(std::ostream& s, const Object& obj, const std::string_view root_name)
    {
        Archiver ar;
        bool ret = false;
        if constexpr (requires { obj.to_container(); })
            ret = ar.save(s, obj.to_container(), root_name);
        else if constexpr (std::is_same_v<std::decay_t<Object>, gpds::container>)
            ret = ar.save(s, obj, root_name);

        return { ret, "" };
    }

    template<typename Archiver, typename Object>
    static
    std::pair<bool, std::string>
    to_stream(std::ostream& s, const Object& obj)
    {
        return to_stream<Archiver, Object>(s, obj, Object::gpds_name);
    }

    template<typename Archiver, typename Object>
    static
    std::pair<bool, std::string>
    to_string(std::string& str, const Object& obj, const std::string_view root_name)
    {
        std::ostringstream ss;

        auto ret = to_stream<Archiver, Object>(ss, obj, root_name);
        str = ss.str();

        return ret;
    }

    template<typename Archiver, typename Object>
    static
    std::pair<bool, std::string>
    to_string(std::string& str, const Object& obj)
    {
        return to_string<Archiver, Object>(str, obj, Object::gpds_name);
    }

    template<typename Archiver, typename Object>
    static
    std::pair<bool, std::string>
    to_file(const std::filesystem::path& path, const Object& obj, const std::string_view root_name)
    {
        std::ofstream file;
        file.open(path, std::ios::out | std::ios::trunc);
        if (!file.is_open())
            return { false, "could not open file" };

        auto ret = to_stream<Archiver, Object>(file, obj, root_name);

        file.close();

        return ret;
    }

    template<typename Archiver, typename Object>
    static
    std::pair<bool, std::string>
    to_file(const std::filesystem::path& path, const Object& obj)
    {
        return to_file<Archiver, Object>(path, obj, Object::gpds_name);
    }

    template<typename Archiver, typename Object>
    static
    std::pair<bool, std::string>
    from_stream(std::istream& s, Object& obj, const std::string_view root_name)
    {
        Archiver ar;
        bool ret = false;
        if constexpr (requires{ obj.from_container(gpds::container{}); }) {
            gpds::container c;
            ret = ar.load(s, c, root_name);
            if (ret)
                obj.from_container(c);
        }
        else if constexpr (std::is_same_v<std::decay_t<Object>, gpds::container>)
            ret = ar.load(s, obj, root_name);

        return { ret, "" };
    }

    template<typename Archiver, typename Object>
    static
    std::pair<bool, std::string>
    from_stream(std::istream& s, Object& obj)
    {
        return from_stream<Archiver, Object>(s, obj, Object::gpds_name);
    }

    // ToDo: No point in accepting std::string_view here!
    template<typename Archiver, typename Object>
    static
    std::pair<bool, std::string>
    from_string(std::string_view str, Object& obj, const std::string_view root_name)
    {
        std::istringstream ss{ std::string{str} };
        return from_stream<Archiver, Object>(ss, obj, root_name);
    }

    template<typename Archiver, typename Object>
    static
    std::pair<bool, std::string>
    from_string(std::string_view str, Object& obj)
    {
        return from_string<Archiver, Object>(str, obj, Object::gpds_name);
    }

    template<typename Archiver, typename Object>
    static
    std::pair<bool, std::string>
    from_file(const std::filesystem::path& path, Object& obj, const std::string_view root_name)
    {
        std::ifstream file;
        file.open(path, std::ios::in);
        if (!file.is_open())
            return { false, "could not open file" };

        auto ret = from_stream<Archiver, Object>(file, obj, root_name);

        file.close();

        return ret;
    }

    template<typename Archiver, typename Object>
    static
    std::pair<bool, std::string>
    from_file(const std::filesystem::path& path, Object& obj)
    {
        return from_file<Archiver, Object>(path, obj, Object::gpds_name);
    }
}
