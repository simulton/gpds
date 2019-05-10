#pragma once

#include <sstream>
#include "../../lib/serialize.h"
#include "address.h"

class Employee : public Gpds::Serialize
{
public:
    Employee() = default;

    virtual Gpds::Container toContainer() const override
    {
        Gpds::Container c;
        c.addValue("first_name", firstName);
        c.addValue("last_name", lastName);
        c.addValue("address", address.toContainer());
        c.addValue("enabled", enabled);

        return c;
    }

    virtual void fromContainer(const Gpds::Container& container) override
    {
        firstName = container.getValue<std::string>("first_name");
        lastName = container.getValue<std::string>("last_name");
        address.fromContainer( *container.getValue<Gpds::Container*>("address") );
        enabled = container.getValue<bool>("enabled");
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
