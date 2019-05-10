#pragma once

#include <sstream>
#include "../../lib/serialize.h"

class Address : public Gpds::Serialize
{
public:
    Address() = default;

    virtual Gpds::Container toContainer() const override
    {
        Gpds::Container c;

        c.addValue("address_1", address1);
        c.addValue("address_2", address2);
        c.addValue("city", city);
        c.addValue("country", country);

        return c;
    }

    virtual void fromContainer(const Gpds::Container& container) override
    {
        address1 = container.getValue<std::string>("address_1");
        address2 = container.getValue<std::string>("address_2");
        city = container.getValue<std::string>("city");
        country = container.getValue<std::string>("country");
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
