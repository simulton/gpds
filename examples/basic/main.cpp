#include <gpds/archiver_xml.hpp>
#include <gpds/archiver_yaml.hpp>

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

#if 0
    // To/from file XML
    {
        const std::filesystem::path filepath = "catalog.xml";

        // Serialize to file
        {
            const auto& [success, msg] = gpds::to_file<gpds::archiver_xml>(filepath, catalog);
            if (!success) {
                std::cerr << "could not store 'catalog' in file: " << msg << std::endl;
                return EXIT_FAILURE;
            }

            std::cout << "successfully serialized 'catalog' to file: " << filepath << std::endl;
        }

        // Deserialize from file
        {
            car_catalog catalog1;
            const auto& [success, msg] = gpds::from_file<gpds::archiver_xml>(filepath, catalog1);
            if (!success) {
                std::cerr << "could not load `catalog` from file: " << msg << std::endl;
                return EXIT_FAILURE;
            }

            std::cout << "successfully deserialized 'catalog' from file: " << filepath << std::endl;
        }
    }

    // To/from file YAML
    {
        const std::filesystem::path filepath = "catalog.yaml";

        // Serialize to file
        {
            const auto& [success, msg] = gpds::to_file<gpds::archiver_yaml>(filepath, catalog);
            if (!success) {
                std::cerr << "could not store 'catalog' in file: " << msg << std::endl;
                return EXIT_FAILURE;
            }

            std::cout << "successfully serialized 'catalog' to file: " << filepath << std::endl;
        }

        // Deserialize from file
        {
            car_catalog catalog1;
            const auto& [success, msg] = gpds::from_file<gpds::archiver_yaml>(filepath, catalog1);
            if (!success) {
                std::cerr << "could not load `catalog` from file: " << msg << std::endl;
                return EXIT_FAILURE;
            }

            std::cout << "successfully deserialized 'catalog' from file: " << filepath << std::endl;
        }
    }
#endif
    // To/from string XML
    {
        std::string str;

        // Serialize to string
        {
            const auto& [success, msg] = gpds::to_string<gpds::archiver_xml>(str, catalog);
            if (!success) {
                std::cerr << "could not store 'catalog' in string: " << msg << std::endl;
                return EXIT_FAILURE;
            }

            std::cout << "successfully serialized 'catalog' to string:\n" << str << std::endl;
        }

        // Deserialize form string
        {
            car_catalog catalog1;
            const auto& [success, msg] = gpds::from_string<gpds::archiver_xml>(str, catalog1);
            if (!success) {
                std::cerr << "could not load `catalog` from string: " << msg << std::endl;
                return EXIT_FAILURE;
            }

            std::cout << "successfully deserialized 'catalog' from string." << std::endl;
        }
    }

    // To/from string YAML
    {
        std::string str;

        // Serialize to string
        {
            const auto& [success, msg] = gpds::to_string<gpds::archiver_yaml>(str, catalog);
            if (!success) {
                std::cerr << "could not store 'catalog' in string: " << msg << std::endl;
                return EXIT_FAILURE;
            }

            std::cout << "successfully serialized 'catalog' to string:\n" << str << std::endl;
        }

        // Deserialize form string
        {
            car_catalog catalog1;
            const auto&[success, msg] = gpds::from_string<gpds::archiver_yaml>(str, catalog1);
            if (!success) {
                std::cerr << "could not load `catalog` from string: " << msg << std::endl;
                return EXIT_FAILURE;
            }

            std::cout << "successfully deserialized 'catalog' from string." << std::endl;
        }
    }

    return 0;
}
