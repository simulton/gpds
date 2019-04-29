#pragma once

#include <sstream>
#include "../../lib/serialize.h"
#include "address.h"

class Employee : public Gds::Serialize
{
public:
    Employee() = default;

    virtual Gds::Container toContainer() const override
    {
        Gds::Container c;
        c.addEntry("first_name", firstName);
        c.addEntry("last_name", lastName);
        c.addEntry("address", address.toContainer());
        c.addEntry("enabled", enabled);

        return c;
    }

    virtual void fromContainer(const Gds::Container& container) override
    {
        firstName = container.getEntry<std::string>("first_name");
        lastName = container.getEntry<std::string>("last_name");
        address.fromContainer( container.getEntry<Gds::Container>("address") );
        enabled = container.getEntry<bool>("enabled");
    }

    std::string toString(const std::string& indentation) const
    {
        std::stringstream s;
        s << std::boolalpha;

        s << indentation << "first name : " << firstName << std::endl;
        s << indentation << "last name  : " << lastName << std::endl;
        s << indentation << "address    : " << std::endl;
        s << address.toString(indentation + indentation) << std::endl;
        s << indentation << "enabled    : " << enabled;

        return s.str();
    }

    std::string firstName;
    std::string lastName;
    Address address;
    bool enabled;
};