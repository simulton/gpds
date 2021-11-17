#include "../test.hpp"

TEST_CASE("containers can be nested")
{
    gpds::container root;
    gpds::container parent;
    gpds::container child;

    child.add_value("name", std::string("John Doe"));

    parent.add_value("child", child);
    REQUIRE(parent.get_value<gpds::container*>("child"));

    root.add_value("child", parent);
    REQUIRE(root.get_value<gpds::container*>("child"));

    std::optional<std::string> str;
    REQUIRE_NOTHROW(str = root.get_value<gpds::container*>("child").value()
        ->get_value<gpds::container*>("child").value()
        ->get_value<std::string>("name")
    );
    REQUIRE_EQ(str, "John Doe");
}
