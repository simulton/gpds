#include "../test.hpp"

TEST_SUITE("container")
{
    struct dut :
        gpds::serialize
    {
        static constexpr const char* gpds_name = "dut";

        struct nested :
            gpds::serialize
        {
            int a;
            std::string foo;

            [[nodiscard]]
            gpds::container
            to_container() const override
            {
                gpds::container c;

                c.add_value("a", a);
                c.add_value("foo", foo);

                return c;
            }

            void
            from_container(const gpds::container& c) override
            {
                a = c.get_value<int>("a").value_or(0);
                foo = c.get_value<std::string>("foo").value_or("n/a");
            }
        };

        nested n;

        [[nodiscard]]
        gpds::container
        to_container() const override
        {
            gpds::container c;

            c.add_value("nested", n.to_container());

            return c;
        }

        void
        from_container(const gpds::container& c) override
        {
            if (const gpds::container* cc = c.get_value<gpds::container*>("nested").value_or(nullptr); cc)
                n.from_container(*cc);
        }
    };

    TEST_CASE("deserialization")
    {
        // Data
        const std::string data = {
            "<dut>"
            "  <nested>"
            "    <a>23</a>"
            "    <foo>Test!</foo>"
            "  </nested>"
            "</dut>"
        };

        // Deserialize
        dut d;
        const auto& [success, msg] = gpds::from_string<gpds::archiver_xml>(data, d);
        REQUIRE(success);

        // Check
        CHECK_EQ(d.n.a, 23);
        CHECK_EQ(d.n.foo, "Test!");
    }

}
