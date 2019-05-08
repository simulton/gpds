#pragma once

#include <forward_list>
#include "../../lib/serialize.h"
#include "car.h"

class CarCatalog : public Gpds::Serialize
{
public:
    std::forward_list<Car> cars;

    virtual Gpds::Container toContainer() const override
    {
        Gpds::Container c;

        for ( const auto& car : cars ) {
            c.addValue( "car", car.toContainer() ).addAttribute("foo", "1.0").setComment("look at my car!");
        }

        return c;
    }

    virtual void fromContainer(const Gpds::Container& c) override
    {
        for (const Gpds::Container* carContainer : c.getValues<Gpds::Container*>( "car" ) ) {
            Car car;
            car.fromContainer( *carContainer );
            cars.push_front( car );
        }
    }
};

