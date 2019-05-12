#pragma once

#include <QTextStream>
#include <QList>
#include "address.h"
#include "employee.h"
#include "../../lib/serialize.h"

class Company : public Gpds::Serialize
{
public:
    Company() = default;

    virtual Gpds::Container toContainer() const override
    {
        // Employees
        Gpds::Container employeesContainer;
        for (const Employee& employee : employees) {
            employeesContainer.addValue("employee", employee.toContainer());
        }

        // Root
        Gpds::Container c;
        c.addValue("name", name);
        c.addValue("random_int", offices);
        c.addValue("random_double", randomDouble);
        c.addValue("address", address.toContainer());
        c.addValue("employees", employeesContainer);

        return c;
    }

    virtual void fromContainer(const Gpds::Container& container) override
    {
        // Employees
        const auto& employeesContainer = container.getValue<Gpds::Container*>("employees");
        for (const auto& employeeContainer : employeesContainer->getValues<Gpds::Container*>("employee")) {
            Employee employee;
            employee.fromContainer( *employeeContainer );
            employees.append( employee );
        }

        // Root
        name = container.getValue<QString>("name");
        offices = container.getValue<int>("random_int");
        randomDouble = container.getValue<double>("random_double");
        address.fromContainer(*container.getValue<Gpds::Container*>("address"));
    }

    void toString(QTextStream& s, const QString& indentation) const
    {
        s << indentation << "name     : " << name;
        s << indentation << "random i : " << offices;
        s << indentation << "random d : " << randomDouble;
        s << indentation << "address";
        address.toString(s, indentation + indentation);
        s << indentation << "employees";
        for (const auto& employee : employees) {
            s << indentation + indentation << "employee";
            employee.toString(s, indentation + indentation + indentation);
        }
    }

    QString name;
    Address address;
    int offices;
    double randomDouble;
    QList<Employee> employees;
};
