#pragma once

#include <QString>
#include "../../lib/serialize.h"

class Color : public Gpds::Serialize
{
public:
    QString name;
    int red;
    int green;
    int blue;

    virtual Gpds::Container toContainer() const override
    {
        Gpds::Container c;

        c.setComment("a color object");
        c.addAttribute("format", "rgb");
        c.addAttribute("name", name.toStdString());

        c.addValue("red", red).addAttribute("depth", "32");
        c.addValue("green", green).addAttribute("depth", "32");
        c.addValue("blue", blue).addAttribute("depth", "32");

        return c;
    }

    virtual void fromContainer(const Gpds::Container& c) override
    {
        // Retrieve format
        const QString& formatString = QString::fromStdString( c.getAttribute("format").value_or("n/a") );
        assert( formatString == "rgb" );

        name = QString::fromStdString( c.getAttribute("name").value_or("n/a") );
        red = c.getValue<int>("red");
        green = c.getValue<int>("green");
        blue = c.getValue<int>("blue");
    }
};
