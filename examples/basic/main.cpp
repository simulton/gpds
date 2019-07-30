#include <iostream>
#include <fstream>
#include <sstream>
#include "archiverxml.h"
#include "carcatalog.h"

int main()
{
    CarCatalog catalog;
    Car car;

    // Create a Jeep
    {
        car.manufacturer = "Jeep";
        car.model = "Grand Cherokee";
        car.year_of_construction = 2009;
        Color color;
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
        Color color;
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
        Color color;
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
    Gpds::ArchiverXml ar;
    ar.save(ofile, catalog.toContainer(), "cars");
    ar.save(ss, catalog.toContainer(), "cars");
    std::cout << ss.str() << std::endl;
    ofile.flush();
    ofile.close();

    std::ifstream ifile;
    ifile.open("data.xml");
    std::string fileContent;
    CarCatalog loadedCatalog;
    ar.load(ifile, loadedCatalog, "cars");
    std::cout << fileContent << std::endl;
    ifile.close();

    return 0;
}