#pragma once

#include <string>
#include "serialize.h"

namespace GpdsTest {

    class Color : public Gpds::Serialize
    {
    public:
        std::string name;
        int red;
        int green;
        int blue;

        Color( ) = default;
        Color( const Color& other ) = default;
        Color( Color&& other ) = default;

        Color& operator=( const Color& rhs ) = default;
        Color& operator=( Color&& rhs ) = default;
        bool operator==( const Color& rhs ) const
        {
            if ( name != rhs.name )
                return false;
            if ( red != rhs.red )
                return false;
            if ( green != rhs.green )
                return false;
            if ( blue != rhs.blue )
                return false;

            return true;
        }

        virtual Gpds::Container toContainer() const override
        {
            Gpds::Container c;

            c.setComment("a color object");
            c.addAttribute("format", "rgb");
            c.addAttribute("name", name);

            c.addValue("red", red).addAttribute("depth", "32");
            c.addValue("green", green).addAttribute("depth", "32");
            c.addValue("blue", blue).addAttribute("depth", "32");

            return c;
        }

        virtual void fromContainer(const Gpds::Container& c) override
        {
            // Retrieve format
            const std::string& formatString = c.getAttribute<std::string>("format").value_or("n/a");

            name = c.getAttribute<std::string>("name").value_or("n/a");
            red = c.getValue<int>("red");
            green = c.getValue<int>("green");
            blue = c.getValue<int>("blue");
        }
    };

}
