#pragma once

#include <string>
#include "../../lib/serialize.h"

class Color : public Gpds::Serialize
{
public:
    std::string name;
    int red;
    int green;
    int blue;

    virtual Gpds::Container toContainer() const override
    {
        Gpds::Container c;

        c.addValue("name", name);
        c.addValue("red", red);
        c.addValue("green", green);
        c.addValue("blue", blue);

        return c;
    }

    virtual void fromContainer(const Gpds::Container& c) override
    {
        name = c.getValue<std::string>("name");
        red = c.getValue<int>("red");
        green = c.getValue<int>("green");
        blue = c.getValue<int>("blue");
    }
};
