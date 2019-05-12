#pragma once

#include <QTextStream>
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
        firstName = container.getValue<QString>("first_name");
        lastName = container.getValue<QString>("last_name");
        address.fromContainer( *container.getValue<Gpds::Container*>("address") );
        enabled = container.getValue<bool>("enabled");
    }

    void toString(QTextStream& s, const QString& indentation) const
    {
        s << indentation << "first name : " << firstName;
        s << indentation << "last name  : " << lastName;
        s << indentation << "address    : ";
        address.toString(s, indentation + indentation);
        s << indentation << "enabled    : ";
    }

    QString firstName;
    QString lastName;
    Address address;
    bool enabled;
};
