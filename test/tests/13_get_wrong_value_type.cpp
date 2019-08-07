#include "catch2/catch.hpp"
#include "../test.h"
#include "value.h"
#include "container.h"
#include <iostream>

TEST_CASE( "retrieving wrong type from value throws an exception" )
{
    Gpds::Value value("1234");
    REQUIRE_THROWS_AS(value.get<int>(), std::bad_variant_access);
}

TEST_CASE( "retrieving wronge type from container" )
{
    Gpds::Container container;

    SECTION( "retrieving integer attribute as string" ) {
        container.addAttribute("number", 1234);
        auto str = container.getAttribute<std::string>("number");
        REQUIRE(str.has_value());
        REQUIRE(str.value() == "1234");
    }

    SECTION( "retrieving string attribute as integer" ) {
        container.addAttribute("text", "1234");
        auto opt = container.getAttribute<int>("text");
        REQUIRE(opt.has_value());
        REQUIRE(opt.value() == 1234);
    }
}

