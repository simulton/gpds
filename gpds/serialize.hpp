#pragma once

#include "container.hpp"
#include "error.hpp"

#include <expected>
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
        /**
         * Destructor.
         */
        virtual
        ~serialize() = default;

        /**
         * Serialize to container.
         * @return The container.
         */
        [[nodiscard]]
        virtual
        gpds::container
        to_container() const = 0;

        /**
         * Deserialize from container.
         * @param container The container.
         */
        virtual
        void
        from_container(const gpds::container& container) = 0;
    };

    template<typename Archiver, typename Object>
    static
    std::expected<void, error>
    to_stream(std::ostream& s, const Object& obj, const std::string_view root_name)
    {
        Archiver ar;
        if constexpr (requires { obj.to_container(); })
            return ar.save(s, obj.to_container(), root_name);
        else if constexpr (std::is_same_v<std::decay_t<Object>, gpds::container>)
            return ar.save(s, obj, root_name);
    }

    template<typename Archiver, typename Object>
    static
    std::expected<void, error>
    to_stream(std::ostream& s, const Object& obj)
    {
        return to_stream<Archiver, Object>(s, obj, Object::gpds_name);
    }

    template<typename Archiver, typename Object>
    static
    std::expected<void, error>
    to_string(std::string& str, const Object& obj, const std::string_view root_name)
    {
        std::ostringstream ss;

        auto ret = to_stream<Archiver, Object>(ss, obj, root_name);
        str = ss.str();

        return ret;
    }

    template<typename Archiver, typename Object>
    static
    std::expected<void, error>
    to_string(std::string& str, const Object& obj)
    {
        return to_string<Archiver, Object>(str, obj, Object::gpds_name);
    }

    template<typename Archiver, typename Object>
    static
    std::expected<void, error>
    to_file(const std::filesystem::path& path, const Object& obj, const std::string_view root_name)
    {
        std::ofstream file;
        file.open(path, std::ios::out | std::ios::trunc);
        if (!file.is_open())
            return std::unexpected(error("could not open file"));

        auto ret = to_stream<Archiver, Object>(file, obj, root_name);

        file.close();

        return ret;
    }

    template<typename Archiver, typename Object>
    static
    std::expected<void, error>
    to_file(const std::filesystem::path& path, const Object& obj)
    {
        return to_file<Archiver, Object>(path, obj, Object::gpds_name);
    }

    template<typename Archiver, typename Object>
    static
    std::expected<void, error>
    from_stream(std::istream& s, Object& obj, const std::string_view root_name)
    {
        Archiver ar;
        if constexpr (requires{ obj.from_container(gpds::container{}); }) {
            gpds::container c;
            auto ret = ar.load(s, c, root_name);
            if (ret)
                obj.from_container(c);

            return ret;
        }
        else if constexpr (std::is_same_v<std::decay_t<Object>, gpds::container>)
            return ar.load(s, obj, root_name);
    }

    template<typename Archiver, typename Object>
    static
    std::expected<void, error>
    from_stream(std::istream& s, Object& obj)
    {
        return from_stream<Archiver, Object>(s, obj, Object::gpds_name);
    }

    template<typename Archiver, typename Object>
    static
    std::expected<void, error>
    from_string(const std::string& str, Object& obj, const std::string_view root_name)
    {
        std::istringstream ss{ str };
        return from_stream<Archiver, Object>(ss, obj, root_name);
    }

    template<typename Archiver, typename Object>
    static
    std::expected<void, error>
    from_string(const std::string& str, Object& obj)
    {
        return from_string<Archiver, Object>(str, obj, Object::gpds_name);
    }

    template<typename Archiver, typename Object>
    static
    std::expected<void, error>
    from_file(const std::filesystem::path& path, Object& obj, const std::string_view root_name)
    {
        std::ifstream file;
        file.open(path, std::ios::in);
        if (!file.is_open())
            return std::unexpected(error("could not open file"));

        auto ret = from_stream<Archiver, Object>(file, obj, root_name);

        file.close();

        return ret;
    }

    template<typename Archiver, typename Object>
    static
    std::expected<void, error>
    from_file(const std::filesystem::path& path, Object& obj)
    {
        return from_file<Archiver, Object>(path, obj, Object::gpds_name);
    }
}
