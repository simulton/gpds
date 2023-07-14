#pragma once

#include "container.hpp"

#include <filesystem>
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
         * The mode of the serialize.
         */
        // ToDo: Remove this once we removed all currently deprecated marked functions using this type.
        enum class mode
        {
            XML,
            YAML,
        };

        virtual
        ~serialize() = default;

        // Container
        [[nodiscard]] virtual gpds::container to_container() const = 0;
        virtual void from_container(const gpds::container& container) = 0;

        [[deprecated("use static gpds::to_string() instead.")]]
        std::pair<bool, std::string>
        to_string(std::string& str, std::string_view root_name, enum mode mode = mode::XML) const;

        [[deprecated("use static gpds::from_string() instead.")]]
        std::pair<bool, std::string>
        from_string(std::string_view str, std::string_view root_name, enum mode mode = mode::XML);

        [[deprecated("use static gpds::to_file() instead.")]]
        std::pair<bool, std::string>
        to_file(const std::filesystem::path& path, std::string_view root_name, enum mode mode = mode::XML) const;

        [[deprecated("use static gpds::from_file() instead.")]]
        std::pair<bool, std::string>
        from_file(const std::filesystem::path& path, std::string_view root_name, enum mode mode = mode::XML);
    };

    template<typename Archiver, typename Object>
    static
    std::pair<bool, std::string>
    to_string(std::string& str, const Object& obj, const std::string_view root_name)
    {
         bool ret = false;

         Archiver ar;
         ret = ar.save(str, obj, root_name);

         return { ret, "" };
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
    from_string(std::string_view str, Object& obj, const std::string_view root_name)
    {
        bool ret = false;

        Archiver ar;
        ret = ar.load(std::string{str}, obj, root_name);     // ToDo: Add interface to archiver accepting std::string_view

        return { ret, "" };
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
    to_file(const std::filesystem::path& path, const Object& obj, const std::string_view root_name)
    {
        bool ret = false;

        Archiver ar;
        ret = ar.save(path, obj, root_name);

        return { ret, "" };
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
    from_file(const std::filesystem::path& path, Object& obj, const std::string_view root_name)
    {
        bool ret = false;

        Archiver ar;
        ret = ar.load(path, obj, root_name);

        return { ret, "" };
    }

    template<typename Archiver, typename Object>
    static
    std::pair<bool, std::string>
    from_file(const std::filesystem::path& path, Object& obj)
    {
        return from_file<Archiver, Object>(path, obj, Object::gpds_name);
    }
}
