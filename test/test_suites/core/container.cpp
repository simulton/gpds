#include "../../test.hpp"
#include "../../mocks.hpp"

TEST_SUITE("core - container")
{
    TEST_CASE("inspection")
    {

        gpds::container c;
        CHECK(c.empty());
        CHECK_FALSE(c.has_values());
        CHECK_FALSE(c.has_attributes());

        SUBCASE("attribute only")
        {
            c.add_attribute("foo", "bar");

            CHECK_FALSE(c.empty());
            CHECK_FALSE(c.has_values());
            CHECK(c.has_attributes());
        }

        SUBCASE("value only")
        {
            c.add_value("v", "k");

            CHECK_FALSE(c.empty());
            CHECK(c.has_values());
            CHECK_FALSE(c.has_attributes());
        }

        SUBCASE("attribute and value")
        {
            c.add_attribute("foo", "bar");
            c.add_value("v", "k");

            CHECK_FALSE(c.empty());
            CHECK(c.has_values());
            CHECK(c.has_attributes());
        }

    }

    TEST_CASE("get gpds::value")
    {
        static constexpr const char* desc_short = "foobar";
        static constexpr const char* desc_long  = "This is a longer description but still something about foobar";

        gpds::container c;
        c.add_value("description", "foobar").add_attribute("type", "short");
        c.add_value("description", "This is a longer description but still foobar").add_attribute("type", "long");


    }

    TEST_CASE("get multiple gpds::value")
    {
        static constexpr const char* desc_short = "foobar";
        static constexpr const char* desc_long  = "This is a longer description but still something about foobar";

        gpds::container c;
        c.add_value("description", desc_short).add_attribute("type", "short");
        c.add_value("description", desc_long).add_attribute("type", "long");

        const auto& values = c.get_values<gpds::value>("description");
        CHECK_EQ(values.size(), 2);

        // Check description short
        CHECK(values.at(0).is_type<std::string>());
        CHECK_EQ(values.at(0).get_attribute<std::string>("type").value_or(""), "short");
        CHECK_EQ(values.at(0).get<std::string>(), desc_short);

        // Check description long
        CHECK(values.at(1).is_type<std::string>());
        CHECK_EQ(values.at(1).get_attribute<std::string>("type").value_or(""), "long");
        CHECK_EQ(values.at(1).get<std::string>(), desc_long);
    }

    TEST_CASE("type decay")
    {
        // Prepare dut
        gpds::container c;
        {
            c.add_value<int>("a", 42);
            c.add_value<double>("b", 4.2);

            gpds::container nc;
            nc.add_value<std::string>("c", "Hello GPDS!");
            c.add_value("nested", std::move(nc));
        }

        SUBCASE("normal")
        {
            CHECK_EQ(c.get_value<int>("a", 0), 42);
            CHECK_EQ(c.get_value<double>("b", 0.0), 4.2);

            const gpds::container* nc = c.get_value<gpds::container*>("nested", nullptr);
            REQUIRE(nc);
            CHECK_EQ(nc->get_value<std::string>("c", ""), "Hello GPDS!");
        }

        SUBCASE("const")
        {
            CHECK_EQ(c.get_value<const int>("a", 0), 42);
            CHECK_EQ(c.get_value<const double>("b", 0.0), 4.2);

            const gpds::container* nc = c.get_value<const gpds::container*>("nested", nullptr);
            REQUIRE(nc);
            CHECK_EQ(nc->get_value<const std::string>("c", ""), "Hello GPDS!");
        }
    }

    TEST_CASE("add_value() / get_value(): Serializable Object")
    {
        SUBCASE("#1")
        {
            test::color c1;
            c1.name = "random";
            c1.r = 42;
            c1.g = 13;
            c1.b = 37;

            // Add
            gpds::container c;
            c.add_value(c1);

            // Get
            auto c2 = c.get_value<test::color>();
            REQUIRE(c2);

            // Check
            CHECK_EQ(c1, *c2);
        }
    }
}
