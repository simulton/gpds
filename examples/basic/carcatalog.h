#pragma once

#include <forward_list>
#include "gpds/serialize.hpp"
#include "car.h"

class car_catalog : public gpds::serialize
{
public:
    std::forward_list<car> cars;

    virtual gpds::container to_container() const override
    {
        gpds::container c;

        c.set_comment("All of our cars:");
        for (const auto& car : cars) {
            c.add_value("car", car.to_container()).set_comment("One of our cars:");
        }

        return c;
    }

    virtual void from_container(const gpds::container& c) override
    {
        for (const gpds::container* carContainer : c.get_values<gpds::container*>("car")) {
            car car;
            car.from_container(*carContainer);
            cars.push_front(car);
        }
    }
};

