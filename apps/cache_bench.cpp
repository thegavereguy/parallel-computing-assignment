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
    std::cerr << "  <function> : 0 for sequential, 1 for parallel, 2 for "
                 "parallel collapse"
              << std::endl;
    return 1;
  }

  int samples = std::stoi(argv[1]);
  int fun     = std::stoi(argv[2]);

  auto counters      = perf::CounterDefinition();
  auto event_counter = perf::EventCounter{counters};
  event_counter.add({"instructions", "cycles", "cache-misses"});

  std::cout << "DIMENSION,INST,CYCLES,CACHE-MISSES" << std::endl;
  for (int n : MAT_TRANS_CASES) {
    Results res;
    for (int i = 0; i < samples; i++) {
      int **A = new int *[n];
      int **B = new int *[n];
      random_allocation(A, n);
      empty_allocation(B, n);

      event_counter.start();
      switch (fun) {
        case 0:
          transpose_sequential(n, A, B);
          break;
        case 1:
          transpose_parallel(n, A, B);
          break;
        case 2:
          transpose_parallel_collapse(n, A, B);
          break;
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
        }
      }
      // std::cout << std::endl;

      deallocate(A, n);
      deallocate(B, n);
    }
    // std::cout << "Mean instructions: " << res.mean_instructions() <<
    // std::endl; std::cout << "Mean cycles: " << res.mean_cycles() <<
    // std::endl; std::cout << "Mean cache misses: " << res.mean_cache_misses()
    // << std::endl;
    std::cout << n << "," << res.mean_instructions(INTERVAL_CONFIDENCE) << ","
              << res.mean_cycles(INTERVAL_CONFIDENCE) << ","
              << res.mean_cache_misses(INTERVAL_CONFIDENCE) << std::endl;
  }

  return 0;
}
