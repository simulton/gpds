#pragma once

#include "car.h"

#include <gpds/container.hpp>

#include <vector>

class car_catalog
{
public:
    static constexpr const char* gpds_name = "car-catalog";

    std::vector<car> cars;

    gpds::container
    to_container() const
    {
        gpds::container c;

        c.add_values(cars);

        return c;
    }

    void
    from_container(const gpds::container& c)
    {
        cars = c.get_values<car>();
    }
};

