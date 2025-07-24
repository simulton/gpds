#pragma once

#include "error.hpp"

#include <expected>
#include <istream>
#include <ostream>

namespace gpds
{

    class container;

    /**
     * Archiver base class.
     *
     * An archiver is responsible for (de)serialization.
     * This class can be derived to implement (de)serialization for various formats such as XML, YAML and JSON.
     */
    class archiver
    {
    public:
        /**
         * Destructor.
         */
        virtual
        ~archiver() = default;

        /**
         * Serialize a container to a stream.
         *
         * @param stream The output stream.
         * @param container The container to serialize.
         * @param root_name The name of the root element.
         * @return Success.
         */
        virtual
        std::expected<void, error>
        save(std::ostream& stream, const container& container, std::string_view root_name) const = 0;

        /**
         * Deserialize a container from a stream.
         *
         * @param stream The input stream.
         * @param container The container to deserialize into.
         * @param root_name The name of the root element.
         * @return Success.
         */
        virtual
        std::expected<void, error>
        load(std::istream& stream, container& container, std::string_view root_name) = 0;
    };

}
