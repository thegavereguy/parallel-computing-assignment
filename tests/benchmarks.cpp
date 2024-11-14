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

TEST_CASE("Matrix transposition parallel with collapse",
          "[mat_trans_par_col]") {
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
