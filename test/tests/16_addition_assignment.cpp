#include "doctest.h"
#include "container.h"

TEST_CASE( "Attributes can be appended" ) {
    Gpds::Attributes attrs1;
    attrs1.add("attr1", 1.234);
    Gpds::Attributes attrs2;
    attrs2.add("attr2", "text");

    attrs1 += attrs2;

    REQUIRE(attrs1.get<double>("attr1") == 1.234);
    REQUIRE(attrs1.get<std::string>("attr2") == "text");
}

TEST_CASE ( "Append attributes with the same name") {
    Gpds::Attributes attrs1;
    attrs1.add("x", 123);
    Gpds::Attributes attrs2;
    attrs2.add("x", "test");

    attrs1 += attrs2;

    REQUIRE(attrs1.get<int>("x") == 123);
}

TEST_CASE( "Containers can be appended" ) {
    Gpds::Container container1;
    container1.addValue("number", 123);
    container1.addAttribute("string", "c");
    Gpds::Container subContainer;
    subContainer.addAttribute("bool", false);
    container1.addValue("subcontainer", subContainer);

    Gpds::Container container2;
    container2.addValue("value", 321);

    container2 += container1;

    REQUIRE(container2.getValue<int>("value") == 321);
    REQUIRE(container2.getValue<int>("number") == 123);
    REQUIRE(container2.getAttribute<std::string>("string") == "c");
    Gpds::Container* sub = container2.getValue<Gpds::Container*>("subcontainer");
    REQUIRE(sub);
    REQUIRE(sub->getValue<bool>("bool") == false);
}

