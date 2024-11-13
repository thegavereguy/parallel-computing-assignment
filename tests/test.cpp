#include <lib/lib.h>
#include <lib/matrix.h>
#include <lib/tools.h>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/benchmark/catch_chronometer.hpp>
#include <catch2/benchmark/catch_clock.hpp>
#include <catch2/catch_get_random_seed.hpp>
#include <catch2/catch_test_case_info.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <catch2/reporters/catch_reporter_streaming_base.hpp>
#include <cstring>
#include <iostream>

const int MAT_TRANS_CASES[12] = {2,   4,   8,   16,   32,   64,
                                 128, 256, 512, 1024, 2048, 4096};

TEST_CASE("Matrix transposition sequential", "[mat_trans_seq]") {
  int n   = 10;
  int** A = new int*[n];
  int** B = new int*[n];

  for (int i = 0; i < n; i++) {
    A[i] = new int[n];
    B[i] = new int[n];
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
  deallocate(A, n);
  deallocate(B, n);

  char* name = new char[100];

  /// Sequential transposition
  for (int i : MAT_TRANS_CASES) {
    // sprintf(name, "mat trans seq (%d)", i);

    sprintf(name, "%d", i);
    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
      int n   = i;
      int** A = new int*[n];
      int** B = new int*[n];
      for (int i = 0; i < n; i++) {
        A[i] = new int[n]();
        B[i] = new int[n]();
        for (int j = 0; j < n; j++) {
          A[i][j] = i * n + j;
        }
      }
      meter.measure([n, A, B] { transpose_sequential(n, A, B); });
      deallocate(A, n);
      deallocate(B, n);
    };
  }

  delete[] name;
}

TEST_CASE("Matrix transposition parallel", "[mat_trans_par]") {
  int n   = 10;
  int** A = new int*[n];
  int** B = new int*[n];

  for (int i = 0; i < n; i++) {
    A[i] = new int[n];
    B[i] = new int[n];
    for (int j = 0; j < n; j++) {
      A[i][j] = i * n + j;
    }
  }
  transpose_parallel(n, A, B);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      REQUIRE(A[i][j] == B[j][i]);
    }
  }
  deallocate(A, n);
  deallocate(B, n);

  char* name = new char[100];

  // parallel transposition
  for (int i : MAT_TRANS_CASES) {
    // sprintf(name, "mat trans par (%d)", i);

    sprintf(name, "%d", i);
    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
      int n   = i;
      int** A = new int*[n];
      int** B = new int*[n];
      for (int i = 0; i < n; i++) {
        A[i] = new int[n]();
        B[i] = new int[n]();
        for (int j = 0; j < n; j++) {
          A[i][j] = i * n + j;
        }
      }
      meter.measure([n, A, B] { transpose_parallel(n, A, B); });
      deallocate(A, n);
      deallocate(B, n);
    };
  }

  delete[] name;
}

TEST_CASE("Matrix transposition parallel collapse", "[mat_trans_par_col]") {
  int n   = 10;
  int** A = new int*[n];
  int** B = new int*[n];

  for (int i = 0; i < n; i++) {
    A[i] = new int[n];
    B[i] = new int[n];
    for (int j = 0; j < n; j++) {
      A[i][j] = i * n + j;
    }
  }
  transpose_parallel_collapse(n, A, B);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      REQUIRE(A[i][j] == B[j][i]);
    }
  }
  deallocate(A, n);
  deallocate(B, n);

  char* name = new char[100];

  // parallel transposition
  for (int i : MAT_TRANS_CASES) {
    // sprintf(name, "mat trans par col(%d)", i);
    sprintf(name, "%d", i);

    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
      int n   = i;
      int** A = new int*[n];
      int** B = new int*[n];
      for (int i = 0; i < n; i++) {
        A[i] = new int[n]();
        B[i] = new int[n]();
        for (int j = 0; j < n; j++) {
          A[i][j] = i * n + j;
        }
      }
      meter.measure([n, A, B] { transpose_parallel_collapse(n, A, B); });
      deallocate(A, n);
      deallocate(B, n);
    };
  }

  delete[] name;
}

// define a custom reporter
class PartialCSVReporter : public Catch::StreamingReporterBase {
 public:
  using StreamingReporterBase::StreamingReporterBase;

  static std::string getDescription() {
    return "Reporter for benchmarks in CSV format";
  }

  void testCasePartialStarting(Catch::TestCaseInfo const& testInfo,
                               uint64_t partNumber) override {
    // std::cout << "TestCase: " << testInfo.name << '#' << partNumber << '\n';
    std::cout << "DIMENSION, MEAN" << '\n';
  }

  void testCasePartialEnded(Catch::TestCaseStats const& testCaseStats,
                            uint64_t partNumber) override {
    // std::cout << "TestCaseEnded: " << testCaseStats.testInfo->name << '#' <<
    // partNumber << '\n';
  }

  void benchmarkEnded(Catch::BenchmarkStats<> const& stats) override {
    std::cout << stats.info.name << "," << stats.mean.point.count() << '\n';
  }
};

CATCH_REGISTER_REPORTER("csv", PartialCSVReporter)

// TEST_CASE("Array sum", "[array_sum]") {
//   int n  = 10;
//   int* v = new int[n];
//   for (int i = 0; i < n; i++) {
//     v[i] = i;
//   }
//   REQUIRE(array_sum_sequential(v, n) == 45);
//   REQUIRE(array_sum_parallel(v, n) == 45);
//
//   delete[] v;
//
//   int test[6] = {10, 100, 1000, 10000, 100000, 1000000};
//   char* name  = new char[100];
//   for (int value : test) {
//     sprintf(name, "array sum seq (%d)", value);
//     BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
//       int n  = value;
//       int* v = new int[n];
//       for (int i = 0; i < n; i++) {
//         v[i] = i;
//       }
//       meter.measure([v, n] { return array_sum_sequential(v, n); });
//       delete[] v;
//     };
//   }
//   for (int value : test) {
//     sprintf(name, "array sum par (%d)", value);
//     BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
//       int n  = value;
//       int* v = new int[n];
//       for (int i = 0; i < n; i++) {
//         v[i] = i;
//       }
//       meter.measure([v, n] { return array_sum_parallel(v, n); });
//       delete[] v;
//     };
//   }
//   delete[] name;
// }
