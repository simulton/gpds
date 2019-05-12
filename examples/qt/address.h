#pragma once

#include <QTextStream>
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
        address1 = container.getValue<QString>("address_1");
        address2 = container.getValue<QString>("address_2");
        city = container.getValue<QString>("city");
        country = container.getValue<QString>("country");
    }

    void toString(QTextStream& s, const QString& indentation) const
    {
        s << indentation << "address 1 : " << address1;
        s << indentation << "address 2 : " << address2;
        s << indentation << "city      : " << city;
        s << indentation << "country   : " << country;
    }

    QString address1;
    QString address2;
    QString city;
    QString country;
};
