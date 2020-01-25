#pragma once

#include <filesystem>
#include <fstream>
#include "gpds_export.h"
#include "serialize.hpp"

#ifdef GPDS_FEATURE_XPATH
    #include "document.hpp"
#endif

namespace gpds
{

    class GPDS_EXPORT archiver
    {
    public:
        virtual ~archiver() = default;

        virtual bool save(std::ostream& stream, const container& container, const std::string& rootName) const = 0;

        bool save(std::ostream& stream, const serialize& object, const std::string& rootName) const
        {
            return save(stream, object.to_container(), rootName);
        }

        bool save(const std::filesystem::path& path, const container& container, const std::string& rootName) const
        {
            std::ofstream file;
            file.open(path, std::ios::out | std::ios::trunc);
            if (not file.is_open()) {
                return false;
            }
            bool ret = save(file, container, rootName);
            file.close();
            return ret;
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
            if (not file.is_open()) {
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
            if (not success)
                return false;

            object.from_container(c);

            return true;
        }

#ifdef GPDS_FEATURE_XPATH
        virtual std::unique_ptr<document> load(std::istream& stream) = 0;

        virtual std::unique_ptr<document> load(const std::filesystem::path& path)
        {
            // Open file
            std::ifstream file;
            file.open(path, std::ios::in);
            if (not file.is_open())
                return nullptr;

            // Load
            auto doc = load(file);

            // Close file
            file.close();

            return doc;
        }
#endif
    };

}
