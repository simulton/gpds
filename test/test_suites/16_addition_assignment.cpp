#include "gpds/container.hpp"

#include "../test.hpp"

TEST_CASE("Attributes can be appended")
{
    gpds::attributes attrs1;
    attrs1.add("attr1", 1.234);
    gpds::attributes attrs2;
    attrs2.add("attr2", "text");

    attrs1 += attrs2;

    REQUIRE(attrs1.get<double>("attr1") == 1.234);
    REQUIRE(attrs1.get<std::string>("attr2") == "text");
}

TEST_CASE("Append attributes with the same name")
{
    gpds::attributes attrs1;
    attrs1.add("x", 123);
    gpds::attributes attrs2;
    attrs2.add("x", "test");

    attrs1 += attrs2;

    REQUIRE(attrs1.get<int>("x") == 123);
}

TEST_CASE("Containers can be appended")
{
    gpds::container container1;
    container1.add_value("number", 123);
    container1.add_attribute("string", "c");
    gpds::container subContainer;
    subContainer.add_attribute("bool", false);
    container1.add_value("subcontainer", subContainer);

    gpds::container container2;
    container2.add_value("value", 321);

    container2 += container1;

    REQUIRE(container2.get_value<int>("value") == 321);
    REQUIRE(container2.get_value<int>("number") == 123);
    REQUIRE(container2.get_attribute<std::string>("string") == "c");
    gpds::container* sub = nullptr;
    REQUIRE_NOTHROW(sub = container2.get_value<gpds::container*>("subcontainer").value());
    REQUIRE(sub);
    auto attr = sub->get_attribute<bool>("bool");
    REQUIRE(attr);
    REQUIRE(attr == false);
}

