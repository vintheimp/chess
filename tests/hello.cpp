#include <catch2/catch.hpp>
#include <string>

std::string hello() {
    return "hello world";
}

TEST_CASE("Hello world", "Is testing framework alive?") {
    REQUIRE(hello() == "hello world");
}