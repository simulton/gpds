#pragma once

#include <string>
#include "serialize.h"
#include "color.h"

class car : public gpds::serialize
{
public:
    std::string manufacturer;
    std::string model;
    int year_of_construction;
    color color;

    virtual gpds::container to_container() const override
    {
        gpds::container c;

        c.add_value("manufacturer", manufacturer);
        c.add_value("model", model);
        c.add_value("year_of_construction", year_of_construction);
        c.add_value("color", color.to_container());

        return c;
    }

    virtual void from_container(const gpds::container& c) override
    {
        manufacturer = c.get_value<std::string>("manufacturer");
        model = c.get_value<std::string>("model");
        year_of_construction = c.get_value<int>("year_of_construction");
        color.from_container(*c.get_value<gpds::container*>("color"));
    }
};
