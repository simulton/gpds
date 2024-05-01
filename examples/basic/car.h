#pragma once

#include "color.h"

#include <gpds/container.hpp>

#include <string>

class car
{
public:
    constexpr static const char* gpds_name = "car";

    std::string manufacturer;
    std::string model;
    int year_of_construction;
    class color color;

    gpds::container
    to_container() const
    {
        gpds::container c;

        c.add_value("manufacturer", manufacturer);
        c.add_value("model", model);
        c.add_value("year_of_construction", year_of_construction);
        c.add_value("color", color.to_container());

        return c;
    }

    void
    from_container(const gpds::container& c)
    {
        manufacturer = c.get_value<std::string>("manufacturer").value_or("");
        model = c.get_value<std::string>("model").value_or("");
        year_of_construction = c.get_value<int>("year_of_construction").value_or(0);

        if (const gpds::container* c_color = c.get_value<gpds::container*>("color").value_or(nullptr); c_color)
            color.from_container(*c_color);
    }
};
