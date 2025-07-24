#include "../../test.hpp"

#include <gpds/error.hpp>

TEST_SUITE("core - error")
{
    TEST_CASE("construction")
    {
        SUBCASE("default")
        {
            gpds::error e;

            CHECK_FALSE(e.is_error());
            CHECK_FALSE(e);
            CHECK_EQ(e.message(), "");
        }

        SUBCASE("copy")
        {
            SUBCASE("truthy")
            {
                gpds::error e1("foo");
                gpds::error e2(e1);

                // Check e1
                CHECK(e1.is_error());
                CHECK(e1);
                CHECK_EQ(e1.message(), "foo");

                // Check e2
                CHECK(e2.is_error());
                CHECK(e2);
                CHECK_EQ(e2.message(), "foo");
            }

            SUBCASE("falsy")
            {
                gpds::error e1;
                gpds::error e2(e1);

                // Check e1
                CHECK_FALSE(e1.is_error());
                CHECK_FALSE(e1);
                CHECK_EQ(e1.message(), "");

                // Check e2
                CHECK_FALSE(e2.is_error());
                CHECK_FALSE(e2);
                CHECK_EQ(e2.message(), "");
            }
        }

        SUBCASE("move")
        {
            SUBCASE("truthy")
            {
                gpds::error e1("foo");
                gpds::error e2(std::move(e1));

                // Check e1
                CHECK_FALSE(e1.is_error());
                CHECK_FALSE(e1);
                CHECK_EQ(e1.message(), "");

                // Check e2
                CHECK(e2.is_error());
                CHECK(e2);
                CHECK_EQ(e2.message(), "foo");
            }

            SUBCASE("falsy")
            {
                gpds::error e1;
                gpds::error e2(std::move(e1));

                // Check e1
                CHECK_FALSE(e1.is_error());
                CHECK_FALSE(e1);
                CHECK_EQ(e1.message(), "");

                // Check e2
                CHECK_FALSE(e2.is_error());
                CHECK_FALSE(e2);
                CHECK_EQ(e2.message(), "");
            }
        }
    }
}
