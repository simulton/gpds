#include "../test.hpp"

TEST_SUITE("cdata")
{
    template<bool UseCData>
    struct dut :
        gpds::serialize
    {
        static constexpr const char* gpds_name = "dut";

        std::string one;
        std::string two;
        std::string three;

        [[nodiscard]]
        gpds::container
        to_container() const override
        {
            gpds::container c;

            c.add_value("one",   one).set_use_cdata(UseCData);
            c.add_value("two",   two).set_use_cdata(UseCData);
            c.add_value("three", three).set_use_cdata(UseCData);

            return c;
        }

        void
        from_container(const gpds::container& c) override
        {
            one   = c.get_value<std::string>("one").value_or("n/a");
            two   = c.get_value<std::string>("two").value_or("n/a");
            three = c.get_value<std::string>("three").value_or("n/a");
        }
    };

    TEST_CASE("enabled")
    {
        SUBCASE("serialization")
        {
            // Data
            dut<true> d;
            d.one   = "foo";
            d.two   = "bar";
            d.three = "zbar";

            // Serialize
            std::string str;
            const auto& [success, msg] = gpds::to_string<gpds::archiver_xml>(str, d);
            REQUIRE(success);

            // Crude checking of string contents
            auto pos = str.find("<![CDATA[foo]]");
            CHECK_NE(pos, std::string::npos);
            pos = str.find("<![CDATA[bar]]");
            CHECK_NE(pos, std::string::npos);
            pos = str.find("<![CDATA[zbar]]");
            CHECK_NE(pos, std::string::npos);
        }

        SUBCASE("deserialization")
        {
            // Data
            const std::string data = {
                "<dut>"
                "  <one><![CDATA[some random string!]]></one>"
                "  <two><![CDATA[73 is a number]]></two>"
                "  <three>This is without CDATA.</three>"
                "</dut>"
            };

            // Deserialize
            dut<true> d;
            const auto& [success, msg] = gpds::from_string<gpds::archiver_xml>(data, d);
            //REQUIRE_MESSAGE(success, msg.c_str());
            REQUIRE(success);

            // Check
            CHECK_EQ(d.one,   "some random string!");
            CHECK_EQ(d.two,   "73 is a number");
            CHECK_EQ(d.three, "This is without CDATA.");
        }
    }

    TEST_CASE("disabled")
    {
        SUBCASE("serialization")
        {
            // Data
            dut<false> d;
            d.one   = "foo";
            d.two   = "bar";
            d.three = "zbar";

            // Serialize
            std::string str;
            const auto& [success, msg] = gpds::to_string<gpds::archiver_xml>(str, d);
            REQUIRE(success);

            // Crude checking of string contents
            auto pos = str.find("<![CDATA[foo]]");
            CHECK_EQ(pos, std::string::npos);
            pos = str.find("<![CDATA[bar]]");
            CHECK_EQ(pos, std::string::npos);
            pos = str.find("<![CDATA[zbar]]");
            CHECK_EQ(pos, std::string::npos);
        }

        SUBCASE("deserialization")
        {
            // Data
            const std::string data = {
                "<dut>"
                "  <one><![CDATA[some random string!]]></one>"
                "  <two><![CDATA[73 is a number]]></two>"
                "  <three>This is without CDATA.</three>"
                "</dut>"
            };

            // Deserialize
            dut<false> d;
            const auto& [success, msg] = gpds::from_string<gpds::archiver_xml>(data, d);
            //REQUIRE_MESSAGE(success, msg.c_str());
            REQUIRE(success);

            // Check
            CHECK_EQ(d.one,   "some random string!");
            CHECK_EQ(d.two,   "73 is a number");
            CHECK_EQ(d.three, "This is without CDATA.");
        }
    }
}
