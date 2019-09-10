#include "doctest.h"
#include "../test.h"
#include "serialize.h"
#include <iostream>

TEST_CASE( "containers can be nested" )
{
    Gpds::Container root;
    Gpds::Container parent;
    Gpds::Container child;

    child.addValue("name", std::string("John Doe"));

    parent.addValue("child", child);
    REQUIRE(parent.getValue<Gpds::Container*>("child"));

    root.addValue("child", parent);
    REQUIRE(root.getValue<Gpds::Container*>("child"));

    auto str = root.getValue<Gpds::Container*>("child")->getValue<Gpds::Container*>("child")->getValue<std::string>("name");
    REQUIRE(str == "John Doe");
}
