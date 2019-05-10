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
            employees.emplace_back().fromContainer( *employeeContainer );
        }

        // Root
        name = container.getValue<std::string>("name");
        offices = container.getValue<int>("random_int");
        randomDouble = container.getValue<double>("random_double");
        address.fromContainer(*container.getValue<Gpds::Container*>("address"));

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
