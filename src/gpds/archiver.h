#pragma once

#include <gpds/serialize.h>

namespace Gpds
{

    class Archiver
    {
    public:
        virtual ~Archiver() = default;

        virtual bool save(std::string& string, const Container& container, const std::string& rootName) const = 0;

        bool save(std::string& string, const Serialize& object, const std::string& rootName) const
        {
            return save(string, object.toContainer(), rootName);
        }

        virtual bool load(std::string& string, Container& container, const std::string& rootName) = 0;

        bool load(std::string& string, Serialize& object, const std::string& rootName)
        {
            Gpds::Container container;
            bool err = load(string, container, rootName);
            if (!err) {
                return false;
            }

            object.fromContainer(container);

            return true;
        }
    };

}
