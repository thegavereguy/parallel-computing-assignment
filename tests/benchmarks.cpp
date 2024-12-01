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
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

TEST_CASE("Matrix transposition sequential", "[mat_trans_seq]") {
  char* name = new char[100];

  /// Sequential transposition
  for (int i : MAT_TRANS_CASES) {
    // for (int i = 2; i < 2048; i += 255) {
    sprintf(name, "%d", i);
    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
      int n     = i;
      float** A = new float*[n];
      float** B = new float*[n];

      random_allocation(A, n);
      empty_allocation(B, n);

      meter.measure([n, A, B] { return transpose_sequential(n, A, B); });
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
    // for (int i = 2; i < 2048; i += 255) {
    sprintf(name, "%d", i);
    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
      int n     = i;
      float** A = new float*[n];
      float** B = new float*[n];

      random_allocation(A, n);
      empty_allocation(B, n);
      meter.measure([n, A, B] { return transpose_parallel(n, A, B); });
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
    // for (int i = 2; i < 2048; i += 255) {
    sprintf(name, "%d", i);

    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
      int n     = i;
      float** A = new float*[n];
      float** B = new float*[n];
      random_allocation(A, n);
      empty_allocation(B, n);

      meter.measure([n, A, B] { return transpose_parallel_collapse(n, A, B); });
      deallocate(A, n);
      deallocate(B, n);
    };
  }

  delete[] name;
}

TEST_CASE("Matrix transposition with vectorization", "[mat_trans_vec]") {
  char* name = new char[100];

  // parallel transposition
  for (int i : MAT_TRANS_CASES) {
    // for (int i = 2; i < 2048; i += 255) {
    sprintf(name, "%d", i);

    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
      int n    = i;
      float* A = new float[n * n];
      float* B = new float[n * n];
      random_allocation_contiguous(A, n);

      meter.measure([n, A, B] { return transpose_vec(n, A, B); });

      delete[] A;
      delete[] B;
    };
  }

  delete[] name;
}
TEST_CASE("Matrix transposition parallel with unroll", "[mat_trans_par_unr]") {
  char* name = new char[100];

  // parallel transposition
  for (int i : MAT_TRANS_CASES) {
    // for (int i = 2; i < 2048; i += 255) {
    sprintf(name, "%d", i);

    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
      int n     = i;
      float** A = new float*[n];
      float** B = new float*[n];
      random_allocation(A, n);
      empty_allocation(B, n);

      meter.measure([n, A, B] { return transpose_parallel_unroll(n, A, B); });
      deallocate(A, n);
      deallocate(B, n);
    };
  }

  delete[] name;
}
TEST_CASE("Matrix transposition parallel block tiling", "[mat_trans_par_blk]") {
  char* name = new char[100];

  // parallel transposition
  for (int i : MAT_TRANS_CASES) {
    // for (int i = 2; i < 2048; i += 255) {
    sprintf(name, "%d", i);

    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
      int n    = i;
      float* A = new float[n * n];
      float* B = new float[n * n];
      random_allocation_contiguous(A, n);

      meter.measure([n, A, B] { return transpose_parallel_block(n, A, B); });
      delete[] A;
      delete[] B;
    };
  }

  delete[] name;
}

TEST_CASE("Matrix transposition parallel with sse instrinsics",
          "[mat_trans_par_sse]") {
  char* name = new char[100];

  // parallel transposition
  for (int i : MAT_TRANS_CASES) {
    // for (int i = 2; i < 2048; i += 255) {
    sprintf(name, "%d", i);

    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
      int n    = i;
      float* A = new float[n * n];
      float* B = new float[n * n];
      random_allocation_contiguous(A, n);

      meter.measure([n, A, B] { return transpose_parallel_sse(n, A, B); });
      delete[] A;
      delete[] B;
    };
  }

  delete[] name;
}
TEST_CASE("Symmetry check sequential", "[sym_check_seq]") {
  char* name = new char[100];

  // parallel transposition
  for (int i : MAT_TRANS_CASES) {
    // for (int i = 2; i < 2048; i += 255) {
    sprintf(name, "%d", i);

    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
      int n     = i;
      float** A = new float*[n];
      random_allocation(A, n);
      symmetrize(n, A);

      meter.measure([n, A] { return symmetry_check_sequential(n, A); });
      deallocate(A, n);
    };
  }

  delete[] name;
}
TEST_CASE("Symmetry check parallel", "[sym_check_par]") {
  char* name = new char[100];

  // parallel transposition
  for (int i : MAT_TRANS_CASES) {
    // for (int i = 2; i < 2048; i += 255) {
    sprintf(name, "%d", i);

    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
      int n     = i;
      float** A = new float*[n];
      random_allocation(A, n);
      symmetrize(n, A);

      meter.measure([n, A] { return symmetry_check_parallel(n, A); });
      deallocate(A, n);
    };
  }

  delete[] name;
}

TEST_CASE("Symmetry check parallel collapse", "[sym_check_par_col]") {
  char* name = new char[100];

  // parallel transposition
  for (int i : MAT_TRANS_CASES) {
    // for (int i = 2; i < 2048; i += 255) {
    sprintf(name, "%d", i);

    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
      int n     = i;
      float** A = new float*[n];
      random_allocation(A, n);
      symmetrize(n, A);

      meter.measure([n, A] { return symmetry_check_collapse(n, A); });
      deallocate(A, n);
    };
  }

  delete[] name;
}

TEST_CASE("Symmetry check unroll", "[sym_check_unr]") {
  char* name = new char[100];

  // parallel transposition
  for (int i : MAT_TRANS_CASES) {
    // for (int i = 2; i < 2048; i += 255) {
    sprintf(name, "%d", i);

    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
      int n     = i;
      float** A = new float*[n];
      random_allocation(A, n);
      symmetrize(n, A);

      meter.measure([n, A] { return symmetry_check_unroll(n, A); });
      deallocate(A, n);
    };
  }

  delete[] name;
}

TEST_CASE("Symmetry check block", "[sym_check_blk]") {
  char* name = new char[100];

  // parallel transposition
  for (int i : MAT_TRANS_CASES) {
    // for (int i = 2; i < 2048; i += 255) {
    sprintf(name, "%d", i);

    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) {
      int n     = i;
      float** A = new float*[n];
      random_allocation(A, n);
      symmetrize(n, A);

      meter.measure([n, A] { return symmetry_check_block(n, A); });
      deallocate(A, n);
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
    std::cout << "DIMENSION,MEAN,MINT,MAXT,ITER" << '\n';
  }

  void testCasePartialEnded(Catch::TestCaseStats const& testCaseStats,
                            uint64_t partNumber) override {
    // std::cout << "TestCaseEnded: " << testCaseStats.testInfo->name << '#' <<
    // partNumber << '\n';
  }

  void benchmarkEnded(Catch::BenchmarkStats<> const& stats) override {
    std::cout << stats.info.name << "," << stats.mean.point.count() / 1e6 << ","
              << stats.mean.lower_bound.count() / 1e6 << ","
              << stats.mean.upper_bound.count() / 1e6 << ","
              << stats.info.iterations << '\n';
  }
};
CATCH_REGISTER_REPORTER("csv", PartialCSVReporter)
