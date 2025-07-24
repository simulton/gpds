#include <gpds/serialize.hpp>
#if GPDS_ENABLE_XML
    #include <gpds/archiver_xml.hpp>
#endif
#if GPDS_ENABLE_YAML
    #include <gpds/archiver_yaml.hpp>
#endif

#include "carcatalog.h"

#include <fstream>
#include <print>
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
        catalog.cars.push_back(car);
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
        catalog.cars.push_back(car);
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
        catalog.cars.push_back(car);
    }

    // To/from string XML
    #if GPDS_ENABLE_XML
    {
        std::string str;

        // Serialize to string
        {
            const auto& success = gpds::to_string<gpds::archiver_xml>(str, catalog);
            if (!success) {
                std::println("could not store 'catalog' in string: {}", success.error().message());
                return EXIT_FAILURE;
            }

            std::println("successfully serialized 'catalog' to string:\n{}", str);
        }

        // Deserialize form string
        {
            car_catalog catalog1;
            const auto& success = gpds::from_string<gpds::archiver_xml>(str, catalog1);
            if (!success) {
                std::println("could not load `catalog` from string: {}", success.error().message());
                return EXIT_FAILURE;
            }

            std::println("successfully deserialized 'catalog' from string.");
        }
    }
    #endif

    // To/from string YAML
    #if GPDS_ENABLE_YAML
    {
        std::string str;

        // Serialize to string
        {
            const auto& success = gpds::to_string<gpds::archiver_yaml>(str, catalog);
            if (!success) {
                std::println("could not store 'catalog' in string: {}", success.error().message());
                return EXIT_FAILURE;
            }

            std::println("successfully serialized 'catalog' to string:\n{}", str);
        }

        // Deserialize form string
        {
            car_catalog catalog1;
            const auto& success = gpds::from_string<gpds::archiver_yaml>(str, catalog1);
            if (!success) {
                std::println("could not load `catalog` from string: {}", success.error().message());
                return EXIT_FAILURE;
            }

            std::println("successfully deserialized 'catalog' from string.");
        }
    }
    #endif

    return 0;
}
