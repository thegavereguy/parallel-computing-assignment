#include <lib/lib.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Compute sums", "[sum]") {
  REQUIRE(sum(1, 2) == 3);
  REQUIRE(sum(1, 3) == 4);
  REQUIRE(sum(1, 1) == 2);
  REQUIRE(sum(132, 2) == 134);
}

TEST_CASE("Compute products", "[mul]") {
  REQUIRE(mul(1, 2) == 2);
  REQUIRE(mul(1, 3) == 3);
  REQUIRE(mul(1, 1) == 1);
  REQUIRE(mul(132, 2) == 264);
}
