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
  int n     = 16;
  float** A = new float*[n];
  float** B = new float*[n];

  random_allocation(A, n);
  empty_allocation(B, n);

  transpose_sequential(n, A, B);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      REQUIRE(A[i][j] == B[j][i]);
    }
  }
  deallocate(A, n);
  deallocate(B, n);
}
TEST_CASE("Matrix transposition sequential contigous", "[mat_trans_seq_cont]") {
  int n    = 16;
  float* A = new float[n * n];
  float* B = new float[n * n];

  random_allocation_contiguous(A, n);

  transpose_sequential_cont(n, A, B);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      REQUIRE(A[i * n + j] == B[j * n + i]);
    }
  }

  delete[] A;
  delete[] B;
}

TEST_CASE("Matrix transposition parallel", "[mat_trans_par]") {
  int n     = 16;
  float** A = new float*[n];
  float** B = new float*[n];

  random_allocation(A, n);
  empty_allocation(B, n);

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
  int n     = 16;
  float** A = new float*[n];
  float** B = new float*[n];

  random_allocation(A, n);
  empty_allocation(B, n);

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
  int n     = 16;
  float** A = new float*[n];
  float** B = new float*[n];

  random_allocation(A, n);
  empty_allocation(B, n);

  transpose_parallel_unroll(n, A, B);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      REQUIRE(A[i][j] == B[j][i]);
    }
  }
  deallocate(A, n);
  deallocate(B, n);
}

TEST_CASE("Matrix transposition parallel with block tiling",
          "[mat_trans_par_blk]") {
  int n    = 16;
  float* A = new float[n * n];
  float* B = new float[n * n];

  random_allocation_contiguous(A, n);

  transpose_parallel_block_cont(n, A, B);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      REQUIRE(A[i * n + j] == B[j * n + i]);
    }
  }

  delete[] A;
  delete[] B;
}
TEST_CASE("Matrix transposition parallel with block tiling contigous",
          "[mat_trans_seq_cont]") {
  int n    = 16;
  float* A = new float[n * n];
  float* B = new float[n * n];

  random_allocation_contiguous(A, n);

  transpose_parallel_block_cont(n, A, B);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      REQUIRE(A[i * n + j] == B[j * n + i]);
    }
  }

  delete[] A;
  delete[] B;
}

TEST_CASE("Matrix transposition parallel with block sse",
          "[mat_trans_par_sse]") {
  int n     = 16;
  float** A = new float*[n];
  float** B = new float*[n];
  random_allocation(A, n);
  empty_allocation(B, n);

  transpose_parallel_block_sse(n, A, B);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      REQUIRE(A[i][j] == B[j][i]);
    }
  }
  deallocate(A, n);
  deallocate(B, n);
}

TEST_CASE("Check symmetry of matrix transposition - sequential",
          "[sym_check_seq]") {
  SECTION("verify symmetric") {
    int n     = 16;
    float** A = new float*[n];

    random_allocation(A, n);

    symmetrize(n, A);
    REQUIRE(symmetry_check_sequential(n, A));
    deallocate(A, n);
  }

  SECTION("verify asymmetric") {
    int n     = 16;
    float** A = new float*[n];

    random_allocation(A, n);

    symmetrize(n, A);

    A[3][2] = -1;
    REQUIRE_FALSE(symmetry_check_sequential(n, A));
    deallocate(A, n);
  }
}

TEST_CASE("Check symmetry of matrix transposition - parallel",
          "[sym_check_par]") {
  SECTION("verify symmetric") {
    int n     = 16;
    float** A = new float*[n];

    random_allocation(A, n);
    symmetrize(n, A);

    REQUIRE(symmetry_check_parallel(n, A));
    deallocate(A, n);
  }

  SECTION("verify asymmetric") {
    int n     = 16;
    float** A = new float*[n];

    random_allocation(A, n);
    symmetrize(n, A);

    A[3][2] = -1;
    REQUIRE_FALSE(symmetry_check_parallel(n, A));
    deallocate(A, n);
  }
}

TEST_CASE("Check symmetry of matrix transposition - parallel collapse",
          "[sym_check_par_col]") {
  SECTION("verify symmetric") {
    int n     = 16;
    float** A = new float*[n];

    random_allocation(A, n);

    symmetrize(n, A);

    REQUIRE(symmetry_check_collapse(n, A));

    deallocate(A, n);
  }

  SECTION("verify asymmetric") {
    int n     = 16;
    float** A = new float*[n];

    random_allocation(A, n);

    symmetrize(n, A);

    A[3][2] = -1;
    REQUIRE_FALSE(symmetry_check_collapse(n, A));

    deallocate(A, n);
  }
}

TEST_CASE("Check symmetry of matrix transposition - unroll",
          "[sym_check_unr]") {
  SECTION("verify symmetric") {
    int n     = 4;
    float** A = new float*[n];

    random_allocation(A, n);

    symmetrize(n, A);

    REQUIRE(symmetry_check_parallel_unroll(n, A));

    deallocate(A, n);
  }

  SECTION("verify asymmetric") {
    int n     = 16;
    float** A = new float*[n];

    random_allocation(A, n);
    symmetrize(n, A);

    A[3][2] = -1;
    REQUIRE_FALSE(symmetry_check_parallel_unroll(n, A));

    deallocate(A, n);
  }
}
TEST_CASE("Check symmetry of matrix transposition - block", "[sym_check_blk]") {
  SECTION("verify symmetric") {
    int n     = 4;
    float** A = new float*[n];

    random_allocation(A, n);

    symmetrize(n, A);

    REQUIRE(symmetry_check_parallel_block(n, A));

    deallocate(A, n);
  }

  SECTION("verify asymmetric") {
    int n     = 16;
    float** A = new float*[n];

    random_allocation(A, n);
    symmetrize(n, A);

    A[3][2] = -1;
    REQUIRE_FALSE(symmetry_check_parallel_block(n, A));

    deallocate(A, n);
  }
}
TEST_CASE("Check symmetry of matrix transposition - block contiguous",
          "[sym_check_vec]") {
  SECTION("verify symmetric") {
    int n    = 4;
    float* A = new float[n * n];

    random_allocation_contiguous(A, n);

    symmetrize(n, A);

    REQUIRE(symmetry_check_parallel_block_cont(n, A));

    delete[] A;
  }

  SECTION("verify asymmetric") {
    int n    = 4;
    float* A = new float[n * n];

    random_allocation_contiguous(A, n);

    symmetrize(n, A);

    A[6] = -1;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        std::cout << A[i * n + j] << " ";
      }
      std::cout << std::endl;
    }
    REQUIRE_FALSE(symmetry_check_parallel_block_cont(n, A));

    delete[] A;
  }
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
