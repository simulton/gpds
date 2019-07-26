#pragma once

#include <gpds/serialize.h>

namespace Gpds
{

    class Archiver
    {
    public:
        virtual ~Archiver() = default;

        virtual bool save(std::ostream& stream, const Container& container, const std::string& rootName) const = 0;

        bool save(std::ostream& stream, const Serialize& object, const std::string& rootName) const
        {
            return save(stream, object.toContainer(), rootName);
        }

        virtual bool load(std::istream& stream, Container& container, const std::string& rootName) = 0;

        bool load(std::istream& stream, Serialize& object, const std::string& rootName)
        {
            Gpds::Container container;
            bool err = load(stream, container, rootName);
            if (!err) {
                return false;
            }

            object.fromContainer(container);

            return true;
        }
    };

}
