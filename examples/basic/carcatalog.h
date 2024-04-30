#pragma once

#include "car.h"

#include <gpds/serialize.hpp>

#include <forward_list>

class car_catalog :
    public gpds::serialize
{
public:
    static constexpr const char* gpds_name = "car-catalog";

    std::forward_list<car> cars;

    gpds::container
    to_container() const override
    {
        gpds::container c;

        for (const auto& car : cars) {
            c.add_value("car", car.to_container());
        }

        return c;
    }

    void
    from_container(const gpds::container& c) override
    {
        for (const gpds::container* carContainer : c.get_values<gpds::container*>("car")) {
            car car;
            car.from_container(*carContainer);
            cars.push_front(car);
        }
    }
};

