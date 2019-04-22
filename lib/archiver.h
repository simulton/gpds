#pragma once

#include "serialize.h"

namespace Gds
{

    class Archiver
    {
    public:
        virtual ~Archiver() = default;

        virtual bool save(std::ostream& stream, const Serialize& object, const std::string& rootName) const = 0;
        virtual bool load(std::istream& stream, Serialize& object, const std::string& rootName) = 0;
    };

}
