#pragma once

#include <sstream>
#include "../../lib/serialize.h"

class Address : public Gds::Serialize
{
public:
    Address() = default;

    virtual Gds::Container toContainer() const override
    {
        Gds::Container c;

        c.addEntry("address_1", address1);
        c.addEntry("address_2", address2);
        c.addEntry("city", city);
        c.addEntry("country", country);

        return c;
    }

    virtual void fromContainer(const Gds::Container& container) override
    {
        address1 = container.getEntry<std::string>("address_1");
        address2 = container.getEntry<std::string>("address_2");
        city = container.getEntry<std::string>("city");
        country = container.getEntry<std::string>("country");
    }

    std::string toString(const std::string_view& indentation) const
    {
        std::stringstream s;

        s << indentation << "address 1 : " << address1 << std::endl;
        s << indentation << "address 2 : " << address2 << std::endl;
        s << indentation << "city      : " << city << std::endl;
        s << indentation << "country   : " << country;

        return s.str();
    }

    std::string address1;
    std::string address2;
    std::string city;
    std::string country;
};
