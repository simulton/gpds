#include <gpds/archiver_xml.hpp>

#include "carcatalog.h"

#include <iostream>
#include <fstream>
#include <sstream>

int main()
{
    car_catalog catalog;
    car car;

    // Create a Jeep
    {
        car.manufacturer = "Jeep";
        car.model = "Grand Cherokee";
        car.year_of_construction = 2009;
        color color;
        color.name = "Black";
        color.red = 0;
        color.green = 0;
        color.blue = 0;
        car.color = color;
        catalog.cars.push_front(car);
    }

    // Create an Audi
    {
        car.manufacturer = "Audi";
        car.model = "A6";
        car.year_of_construction = 2015;
        color color;
        color.name = "Gray";
        color.red = 50;
        color.green = 50;
        color.blue = 50;
        car.color = color;
        catalog.cars.push_front(car);
    }

    // Create a partially empty car
    {
        car.manufacturer = "";
        car.model = "";
        car.year_of_construction = 0;
        color color;
        color.name = "Baby Vomit Green";
        color.red = 137;
        color.green = 155;
        color.blue = 4;
        car.color = color;
        catalog.cars.push_front(car);
    }

    std::stringstream ss;
    std::ofstream ofile;
    ofile.open("data.xml");
    gpds::archiver_xml ar;
    ar.save(ofile, catalog.to_container(), "cars");
    ar.save(ss, catalog.to_container(), "cars");
    std::cout << ss.str() << std::endl;
    ofile.flush();
    ofile.close();

    std::ifstream ifile;
    ifile.open("data.xml");
    std::string fileContent;
    car_catalog loadedCatalog;
    ar.load(ifile, loadedCatalog, "cars");
    std::cout << fileContent << std::endl;
    ifile.close();

    return 0;
}