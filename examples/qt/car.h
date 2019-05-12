#pragma once

#include <QString>
#include "../../lib/serialize.h"
#include "color.h"

class Car : public Gpds::Serialize {
public:
    QString manufacturer;
    QString model;
    int year_of_construction;
    Color color;

    virtual Gpds::Container toContainer() const override
    {
        Gpds::Container c;

        c.addValue("manufacturer", manufacturer);
        c.addValue("model", model);
        c.addValue("year_of_construction", year_of_construction);
        c.addValue("color", color.toContainer());

        return c;
    }

    virtual void fromContainer(const Gpds::Container &c) override
    {
        manufacturer = c.getValue<QString>("manufacturer");
        model = c.getValue<QString>("model");
        year_of_construction = c.getValue<int>("year_of_construction");
        color.fromContainer( *c.getValue<Gpds::Container*>( "color" ) );
    }
};
