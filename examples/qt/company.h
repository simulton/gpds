#pragma once

#include <vector>
#include <iostream>
#include <sstream>
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
            employeesContainer.addEntry("employee", employee.toContainer());
        }

        // Root
        Gpds::Container c;
        c.addEntry("name", name);
        c.addEntry("random_int", offices);
        c.addEntry("random_double", randomDouble);
        c.addEntry("address", address.toContainer());
        c.addEntry("employees", employeesContainer);

        return c;
    }

    virtual void fromContainer(const Gpds::Container& container) override
    {
        // Employees
        const auto& employeesContainer = container.getEntry<Gpds::Container>("employees");
        for (const auto& employeeContainer : employeesContainer.getEntries<Gpds::Container>("employee")) {
            employees.emplace_back().fromContainer( employeeContainer );
        }

        // Root
        name = container.getEntry<std::string>("name");
        offices = container.getEntry<int>("random_int");
        randomDouble = container.getEntry<double>("random_double");
        address.fromContainer(container.getEntry<Gpds::Container>("address"));

        std::cout << "company: " << std::endl;
        std::cout << toString("  ") << std::endl;
        std::cout << std::endl;
    }

    std::string toString(const std::string& indentation) const
    {
        std::stringstream s;

        s << indentation << "name     : " << name << std::endl;
        s << indentation << "random i : " << offices << std::endl;
        s << indentation << "random d : " << randomDouble << std::endl;
        s << indentation << "address" << std::endl;
        s << address.toString(indentation + indentation) << std::endl;
        s << indentation << "employees" << std::endl;
        for (const auto& employee : employees) {
            s << indentation + indentation << "employee" << std::endl;
            s << employee.toString(indentation + indentation + indentation) << std::endl;
        }

        return s.str();
    }

    std::string name;
    Address address;
    int offices;
    double randomDouble;
    std::vector<Employee> employees;
};
