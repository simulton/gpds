#pragma once

#include <istream>
#include <ostream>

namespace gpds
{

    class container;

    /**
     * Archiver base class.
     *
     * An archiver is responsible for (de)serialization.
     * This class can be derived to implement (de)serialization for various formats such as XML and JSON.
     */
    class archiver
    {
    public:
        archiver() = default;
        archiver(const archiver& other) = delete;
        archiver(archiver&& other) noexcept = delete;
        virtual ~archiver() = default;

        archiver& operator=(const archiver& rhs) = delete;
        archiver& operator=(archiver&& rhs) noexcept = delete;

        virtual
        bool
        save(std::ostream& stream, const container& container, std::string_view rootName) const = 0;

        virtual
        bool
        load(std::istream& stream, container& container, std::string_view rootName) = 0;
    };

}
