#include <lib/lib.h>
#include <lib/matrix.h>
#include <lib/tools.h>
#include <perfcpp/event_counter.h>

#include <iostream>

#define INTERVAL_CONFIDENCE 0

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <samples> <function>" << std::endl;
    std::cerr << "  <samples>  : number of samples to take" << std::endl;
    std::cerr
        << "  <function> : 0 for transpose sequential, 1 for transpose "
           "parallel, 2 for "
           "transpose parallel collapse, 3 for transpose parallel unroll, 4 "
           "for transpose parallel sse, 5 for symmetry check sequential, 6 for "
           "symmetry check parallel, 7 for symmetry check collapse, 8 for "
           "symmetry check unroll, 9 for symmetry check block"
        << std::endl;

    return 1;
  }

  int samples = std::stoi(argv[1]);
  int fun     = std::stoi(argv[2]);

  auto counters      = perf::CounterDefinition();
  auto event_counter = perf::EventCounter{counters};
  event_counter.add(
      {"instructions", "cycles", "cache-references", "cache-misses"});

  std::cout << "DIMENSION,INST,CYCLES,CACHEREF,CACHEMISS" << std::endl;
  for (int n : MAT_TRANS_CASES) {
    Results res;
    for (int i = 0; i < samples; i++) {
      float **A = new float *[n];
      float **B = new float *[n];
      float **C = new float *[n];
      float *D  = new float[n * n];
      float *E  = new float[n * n];
      random_allocation(A, n);
      empty_allocation(B, n);
      random_allocation(C, n);
      random_allocation_contiguous(D, n);
      symmetrize(n, C);
      symmetrize(n, E);

      event_counter.start();
      switch (fun) {
        case 0:
          transpose_sequential(n, A, B);
          break;
        case 1:
          transpose_sequential_cont(n, D, E);
        case 2:
          transpose_parallel(n, A, B);
          break;
        case 3:
          transpose_parallel_collapse(n, A, B);
          break;
        case 4:
          transpose_parallel_unroll(n, A, B);
          break;
        case 5:
          transpose_parallel_block(n, A, B);
          break;
        case 6:
          transpose_parallel_block_cont(n, D, E);
          break;
        case 7:
          transpose_parallel_block_sse(n, A, B);
          break;
        case 8:
          symmetry_check_sequential(n, C);
          break;
        case 9:
          symmetry_check_sequential_cont(n, E);
          break;
        case 10:
          symmetry_check_parallel(n, C);
          break;
        case 11:
          symmetry_check_collapse(n, C);
          break;
        case 12:
          symmetry_check_parallel_unroll(n, C);
          break;
        case 13:
          symmetry_check_parallel_block(n, C);
          break;
        case 14:
          symmetry_check_parallel_block_cont(n, E);
      }

      event_counter.stop();

      /// Print the result to the console
      const auto result = event_counter.result();

      // std::cout << "Sequential(" << n << "): ";
      for (const auto [event_name, value] : result) {
        //  std::cout << event_name << ": " << value << " ";

        if (event_name == "instructions") {
          res.push_back_instructions(value);
        } else if (event_name == "cycles") {
          res.push_back_cycles(value);
        } else if (event_name == "cache-misses") {
          res.push_back_cache_misses(value);
        } else if (event_name == "cache-references") {
          res.push_back_cache_refs(value);
        }
      }
      // std::cout << std::endl;

      deallocate(A, n);
      deallocate(B, n);
      deallocate(C, n);
      delete[] D;
      delete[] E;
    }
    // std::cout << "Mean instructions: " << res.mean_instructions() <<
    // std::endl; std::cout << "Mean cycles: " << res.mean_cycles() <<
    // std::endl; std::cout << "Mean cache misses: " << res.mean_cache_misses()
    // << std::endl;
    std::cout << n << "," << res.mean_instructions(INTERVAL_CONFIDENCE) << ","
              << res.mean_cycles(INTERVAL_CONFIDENCE) << ","
              << res.mean_cache_refs(INTERVAL_CONFIDENCE) << ","
              << res.mean_cache_misses(INTERVAL_CONFIDENCE) << std::endl;
  }

  return 0;
}
