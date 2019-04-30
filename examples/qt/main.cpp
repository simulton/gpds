#include <sstream>
#include <iostream>
#include <QCoreApplication>
#include "../../lib/archiverxml.h"
#include "employee.h"
#include "address.h"
#include "company.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Create employee 1
    Employee employee1;
    employee1.firstName = "John";
    employee1.lastName = "Doe";
    employee1.enabled = false;

    // Create employee 2
    Employee employee2;
    employee2.firstName = "Peter";
    employee2.lastName = "Peterson";
    Address address2;
    address2.address1 = "Bad boys street 420";
    address2.city = "Cracktown";
    address2.country = "Lichtenstein";
    employee2.address = address2;
    employee2.enabled = true;

    // Create employee 3
    Employee employee3;
    employee3.firstName = "Karl";
    employee3.lastName = "Karlson";
    employee3.enabled = true;

    // Create a company
    Address company1Address;
    company1Address.address1 = "Seewjinenstrasse 2";
    company1Address.city = "3930 Visp";
    company1Address.country = "Switzerland";
    Company company1;
    company1.name = "Simulton Holding AG";
    company1.offices = 7;
    company1.randomDouble = 3.1415;
    company1.employees.push_back(employee1);
    company1.employees.push_back(employee2);
    company1.employees.push_back(employee3);
    company1.address = company1Address;

    std::stringstream stream;
    Gpds::ArchiverXml ar;
    ar.save(stream, company1, "company");

    std::cout << stream.str();
    std::cout << std::endl;

    Company restoredCompany;
    ar.load(stream, restoredCompany, "company");

    exit(0);

    return a.exec();
}
