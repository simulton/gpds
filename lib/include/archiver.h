#pragma once

#include "gpds_export.h"
#include "serialize.h"

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
    };

}
