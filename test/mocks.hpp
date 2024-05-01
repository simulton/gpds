#pragma once

#include "../../lib/include/gpds/container.hpp"

#include <string>

namespace test
{

    struct color
    {
        constexpr static const char* gpds_name = "color";

        std::string name;
        int r = 0;
        int g = 0;
        int b = 0;

        color() = default;
        color(const color& other) = default;
        color(color&& other) = default;

        color& operator=(const color& rhs) = default;
        color& operator=(color&& rhs) = default;

        bool
        operator==(const color& rhs) const = default;

        gpds::container
        to_container() const
        {
            gpds::container c;

            c.add_attribute("format", "rgb");
            c.add_attribute("name", name);

            c.add_value("red", r).add_attribute("depth", "32");
            c.add_value("green", g).add_attribute("depth", "32");
            c.add_value("blue", b).add_attribute("depth", "32");

            return c;
        }

        void
        from_container(const gpds::container& c)
        {
            const std::string& format_string = c.get_attribute<std::string>("format").value_or("n/a");

            name = c.get_attribute<std::string>("name").value_or("n/a");
            r = c.get_value<int>("red").value_or(0);
            g = c.get_value<int>("green").value_or(0);
            b = c.get_value<int>("blue").value_or(0);
        }
    };

}
