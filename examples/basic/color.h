#pragma once

#include <gpds/serialize.hpp>

#include <string>

class color :
    public gpds::serialize
{
public:
    std::string name;
    int red;
    int green;
    int blue;

    gpds::container
    to_container() const override
    {
        gpds::container c;

        c.add_attribute("format", "rgb");
        c.add_attribute("name", name);

        c.add_value("red", red).add_attribute("depth", "32");
        c.add_value("green", green).add_attribute("depth", "32");
        c.add_value("blue", blue).add_attribute("depth", "32");

        return c;
    }

    void
    from_container(const gpds::container& c) override
    {
        // Retrieve format
        const std::string& formatString = c.get_attribute<std::string>("format").value_or("n/a");

        name = c.get_attribute<std::string>("name").value_or("n/a");
        red = c.get_value<int>("red").value_or(0);
        green = c.get_value<int>("green").value_or(0);
        blue = c.get_value<int>("blue").value_or(0);
    }
};
