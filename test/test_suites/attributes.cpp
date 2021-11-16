#include "../test.hpp"

TEST_SUITE("attributes")
{
    TEST_CASE("empty")
    {
        gpds::attributes a;
        CHECK(a.empty());

        a.add("foo", "bar");
        CHECK_FALSE(a.empty());
    }

    TEST_CASE("deserialize")
    {

        static const std::string xml_str =
            "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<data>"
            "  <value1 attr=\"true\">"
            "     <dummy/>"
            "  </value1>"
            "  <value2 attr=\"true\"/>"
            "</data>";

        SUBCASE("range based for-loop")
        {

            struct test :
                gpds::serialize
            {
                gpds::container to_container() const override { return { }; }

                void from_container(const gpds::container& c) override
                {
                    WARN_EQ(c.values.size(), 2);
                    for (const gpds::container* cc : c.get_values<gpds::container*>("value")) {
                        REQUIRE_EQ("true", cc->get_attribute<std::string>("attr").value_or(""));
                    }
                }
            };

            test dut;
            gpds_test::test::deserialize(xml_str, dut, "data");
        }

        SUBCASE("structural bindings for-loop")
        {

            struct test :
                    gpds::serialize
            {
                gpds::container to_container() const override { return { }; }

                void from_container(const gpds::container& c) override
                {
                    WARN_EQ(c.values.size(), 2);
                    for (const auto& [name, value] : c.values) {
                        if (name == "value1") {
                            REQUIRE_EQ("true", value.get_attribute<std::string>("attr").value_or(""));
                        }

                        else if (name == "value2") {
                            REQUIRE_EQ("true", value.get_attribute<std::string>("attr").value_or(""));
                        }
                    }
                }
            };

            test dut;
            gpds_test::test::deserialize(xml_str, dut, "data");
        }

    }
}
