#include <lib/matrix.h>
#include <lib/tools.h>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/benchmark/catch_chronometer.hpp>
#include <catch2/benchmark/catch_clock.hpp>
#include <catch2/catch_get_random_seed.hpp>
#include <catch2/catch_test_case_info.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_stringref.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <catch2/reporters/catch_reporter_streaming_base.hpp>
#include <cstring>
#include <iostream>

TEST_CASE("Matrix transposition sequential", "[mat_trans_seq]") {
  int n   = 16;
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
}

TEST_CASE("Matrix transposition parallel", "[mat_trans_par]") {
  int n   = 16;
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
}

TEST_CASE("Matrix transposition parallel with collapse",
          "[mat_trans_par_col]") {
  int n   = 16;
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
}

TEST_CASE("Matrix transposition parallel with unroll", "[mat_trans_par_unr]") {
  int n   = 16;
  int** A = new int*[n];
  int** B = new int*[n];

  for (int i = 0; i < n; i++) {
    A[i] = new int[n];
    B[i] = new int[n];
    for (int j = 0; j < n; j++) {
      A[i][j] = i * n + j;
    }
  }
  transpose_parallel_unroll(n, A, B);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      REQUIRE(A[i][j] == B[j][i]);
    }
  }
  deallocate(A, n);
  deallocate(B, n);
}

TEST_CASE("Matrix transposition with vectorization", "[mat_trans_vec]") {
  int n   = 16;
  int** A = new int*[n];
  int** B = new int*[n];

  for (int i = 0; i < n; i++) {
    A[i] = new int[n];
    B[i] = new int[n];
    for (int j = 0; j < n; j++) {
      A[i][j] = i * n + j;
    }
  }
  transpose_vec(n, A, B);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      REQUIRE(A[i][j] == B[j][i]);
    }
  }
  deallocate(A, n);
  deallocate(B, n);
}

TEST_CASE("Matrix transposition with block sse", "[mat_trans_blk_sse]") {
  int n     = 16;
  float** A = new float*[n];
  float** B = new float*[n];

  for (int i = 0; i < n; i++) {
    A[i] = new float[n];
    B[i] = new float[n];
    for (int j = 0; j < n; j++) {
      A[i][j] = i * n + j;
    }
  }
  transpose_parallel_sse(n, A, B);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      REQUIRE(A[i][j] == B[j][i]);
    }
  }
  deallocate(A, n);
  deallocate(B, n);
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
    std::cout << "TestCase: " << testInfo.name << '#' << partNumber << '\n';
    // std::cout << "DIMENSION, MEAN" << '\n';
  }

  void testCasePartialEnded(Catch::TestCaseStats const& testCaseStats,
                            uint64_t partNumber) override {
    std::cout << "TestCaseEnded: " << testCaseStats.testInfo->name << '#'
              << partNumber << '\n';
  }

  void benchmarkPreparing(Catch::StringRef str) override {
    std::cout << "DIMENSION,MEAN" << '\n';
  }
};

CATCH_REGISTER_REPORTER("csv", PartialCSVReporter)
