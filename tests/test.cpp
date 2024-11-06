#include <lib/lib.h>
#include <lib/matrix.h>

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

TEST_CASE("Matrix transposition", "[matrix]") {
  int n      = 10;
  double** A = new double*[n];
  double** B = new double*[n];
  for (int i = 0; i < n; i++) {
    A[i] = new double[n];
    B[i] = new double[n];
    for (int j = 0; j < n; j++) {
      A[i][j] = i * n + j;
    }
  }
  transpose_sequential(n, A, B);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      REQUIRE(A[i][j] == B[j][i]);
    }
  }

  BENCHMARK_ADVANCED("matrix transposition 10")(
      Catch::Benchmark::Chronometer meter) {
    int n      = 10;
    double** A = new double*[n];
    double** B = new double*[n];
    for (int i = 0; i < n; i++) {
      A[i] = new double[n];
      B[i] = new double[n];
      for (int j = 0; j < n; j++) {
        A[i][j] = i * n + j;
      }
    }
    meter.measure([n, A, B] { transpose_sequential(n, A, B); });
    delete[] A;
    delete[] B;
  };
  BENCHMARK_ADVANCED("matrix transposition 100")(
      Catch::Benchmark::Chronometer meter) {
    int n      = 100;
    double** A = new double*[n];
    double** B = new double*[n];
    for (int i = 0; i < n; i++) {
      A[i] = new double[n];
      B[i] = new double[n];
      for (int j = 0; j < n; j++) {
        A[i][j] = i * n + j;
      }
    }
    meter.measure([n, A, B] { transpose_sequential(n, A, B); });
    delete[] A;
    delete[] B;
  };
  BENCHMARK_ADVANCED("matrix transposition 1000")(
      Catch::Benchmark::Chronometer meter) {
    int n      = 1000;
    double** A = new double*[n];
    double** B = new double*[n];
    for (int i = 0; i < n; i++) {
      A[i] = new double[n];
      B[i] = new double[n];
      for (int j = 0; j < n; j++) {
        A[i][j] = i * n + j;
      }
    }
    meter.measure([n, A, B] { transpose_sequential(n, A, B); });
    delete[] A;
    delete[] B;
  };
  BENCHMARK_ADVANCED("matrix transposition 1500")(
      Catch::Benchmark::Chronometer meter) {
    int n      = 1500;
    double** A = new double*[n];
    double** B = new double*[n];
    for (int i = 0; i < n; i++) {
      A[i] = new double[n];
      B[i] = new double[n];
      for (int j = 0; j < n; j++) {
        A[i][j] = i * n + j;
      }
    }
    meter.measure([n, A, B] { return transpose_sequential(n, A, B); });
    delete[] A;
    delete[] B;
  };
}
