#include <lib/lib.h>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/benchmark/catch_clock.hpp>
#include <catch2/catch_get_random_seed.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdlib>

#include "catch2/benchmark/catch_chronometer.hpp"

TEST_CASE("Compute sums", "[sum]") {
  REQUIRE(sum(1, 2) == 3);
  REQUIRE(sum(1, 3) == 4);
  REQUIRE(sum(1, 1) == 2);
  REQUIRE(sum(132, 2) == 134);

  BENCHMARK("sum(1, 2)") { return sum(1, 2); };
  BENCHMARK("sum(1, 3)") { return sum(1, 3); };
  BENCHMARK("sum(1, 1)") { return sum(1, 1); };
}

TEST_CASE("Compute products", "[mul]") {
  REQUIRE(mul(1, 2) == 2);
  REQUIRE(mul(1, 3) == 3);
  REQUIRE(mul(1, 1) == 1);
  REQUIRE(mul(132, 2) == 264);
  BENCHMARK("mul(1, 2)") { return mul(1, 2); };
  BENCHMARK("mul(1, 3)") { return mul(1, 3); };
  BENCHMARK("mul(1, 1)") { return mul(1, 1); };
  BENCHMARK("mul(132, 2)") { return mul(132, 2); };
}

TEST_CASE("Merge sort check", "[merge_sort]") {
  int n = 6;
  srand(Catch::getSeed());

  double* arr = new double[n];

  for (int i = 0; i < n; i++) {
    arr[i] = rand() % 1000;
  }

  merge_sort(arr, 0, n - 1);
  for (int i = 0; i < n - 1; i++) {
    REQUIRE(arr[i] <= arr[i + 1]);
  }
  BENCHMARK("merge_sort") { merge_sort(arr, 0, n - 1); };
}

TEST_CASE("Merge sort bench", "[merge_sort]") {
  int n = 1000;
  srand(Catch::getSeed());
  int* arr = new int[n];
  for (int i = 0; i < n; i++) {
    arr[i] = rand() % 100000;
  }

  // BENCHMARK("merge_sort") { merge_sort(arr, 0, n - 1); };
  BENCHMARK_ADVANCED("merge_sort 1000")(Catch::Benchmark::Chronometer meter) {
    int n = 1000;
    srand(Catch::getSeed());
    double* arr = new double[n];
    for (int i = 0; i < n; i++) {
      arr[i] = rand() % 100000;
    }
    meter.measure([arr, n] { merge_sort(arr, 0, n - 1); });
  };
  BENCHMARK_ADVANCED("merge_sort 10000")(Catch::Benchmark::Chronometer meter) {
    int n = 10000;
    srand(Catch::getSeed());
    double* arr = new double[n];
    for (int i = 0; i < n; i++) {
      arr[i] = rand() % 10000;
    }
    meter.measure([arr, n] { merge_sort(arr, 0, n - 1); });
  };

  BENCHMARK_ADVANCED("merge_sort 1000000")(
      Catch::Benchmark::Chronometer meter) {
    int n = 1000000;
    srand(Catch::getSeed());
    double* arr = new double[n];
    for (int i = 0; i < n; i++) {
      arr[i] = rand() % (n * 100);
    }
    meter.measure([arr, n] { merge_sort(arr, 0, n - 1); });
  };
}
