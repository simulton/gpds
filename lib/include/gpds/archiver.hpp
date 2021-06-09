#pragma once

#include <filesystem>
#include <fstream>
#include "gpds_export.hpp"
#include "serialize.hpp"

namespace gpds
{

    /**
     * Archiver base class.
     *
     * An archiver is responsible for (de)serialization.
     * This class can be derived to implement (de)serialization for various formats such as XML and JSON.
     */
    class GPDS_EXPORT archiver
    {
    public:
        archiver() = default;
        archiver(const archiver& other) = delete;
        archiver(archiver&& other) noexcept = delete;
        virtual ~archiver() = default;

        archiver& operator=(const archiver& rhs) = delete;
        archiver& operator=(archiver&& rhs) noexcept = delete;

        virtual bool save(std::ostream& stream, const container& container, const std::string& rootName) const = 0;

        bool save(std::ostream& stream, const serialize& object, const std::string& rootName) const
        {
            return save(stream, object.to_container(), rootName);
        }

        bool save(const std::filesystem::path& path, const container& container, const std::string& rootName) const
        {
            std::ofstream file;
            file.open(path, std::ios::out | std::ios::trunc);
            if (!file.is_open()) {
                return false;
            }
            bool ret = save(file, container, rootName);
            file.close();
            return ret;
        }

        bool save(const std::filesystem::path& path, const serialize& object, const std::string& root_name) const
        {
            return save(path, object.to_container(), root_name);
        }

        bool save(std::string& str, const container& c, const std::string& root_name) const
        {
            std::ostringstream ss{ str };
            return save(ss, c, root_name);
        }

        bool save(std::string& str, const serialize& obj, const std::string& root_name) const
        {
            return save(str, obj.to_container(), root_name);
        }

        virtual bool load(std::istream& stream, container& container, const std::string& rootName) = 0;

        bool load(std::istream& stream, serialize& object, const std::string& rootName)
        {
            gpds::container container;
            bool err = load(stream, container, rootName);
            if (!err) {
                return false;
            }

            object.from_container(container);

            return true;
        }

        bool load(const std::filesystem::path& path, container& container, const std::string& rootName)
        {
            std::ifstream file;
            file.open(path, std::ios::in);
            if (!file.is_open()) {
                return false;
            }
            bool ret = load(file, container, rootName);
            file.close();
            return ret;
        }

        bool load(const std::filesystem::path& path, serialize& object, const std::string& root_name)
        {
            gpds::container c;
            const bool success = load(path, c, root_name);
            if (!success)
                return false;

            object.from_container(c);

            return true;
        }

        bool load(const std::string& str, container& c, const std::string& root_name)
        {
            std::istringstream ss{ str };
            return load(ss, c, root_name);
        }

        bool load(const std::string& str, serialize& obj, const std::string& root_name)
        {
            gpds::container c;
            const bool successful = load(str, c, root_name);
            if (!successful)
                return false;

            obj.from_container(c);

            return true;
        }

    };

}
