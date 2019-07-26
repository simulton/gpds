#include <sstream>
#include <QtDebug>
#include <QCoreApplication>
#include <gpds/archiverxml.h>
#include "car.h"
#include "color.h"
#include "carcatalog.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

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

    std::stringstream stream;
    Gpds::ArchiverXml ar;

    // Serialize
    {
        ar.save(stream, catalog, "cars");

        qDebug() << "--- Serialized data: ---";
        qDebug() << stream.str().c_str();
    }

    // Deserialize
    {
        CarCatalog restoredCatalog;
        ar.load(stream, restoredCatalog, "cars");

        QString str;
        QTextStream s(&str);
        //restoredCatalog.toString(s, "");
        qDebug() << "--- Deserialized data: ---";
        qDebug() << s.readAll();
    }

    return a.exec();
}
