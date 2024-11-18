#include <lib/lib.h>
#include <lib/matrix.h>
#include <lib/tools.h>
#include <perfcpp/event_counter.h>

#include <iostream>

int main(int argc, char **argv) {
  int samples = std::stoi(argv[1]);

  auto counters      = perf::CounterDefinition();
  auto event_counter = perf::EventCounter{counters};
  event_counter.add({"instructions", "cycles", "cache-misses"});

  for (int n : MAT_TRANS_CASES) {
    int **A = new int *[n];
    int **B = new int *[n];
    random_allocation(A, n);
    empty_allocation(B, n);

    event_counter.start();
    for (int i = 0; i < samples; i++) {
      transpose_sequential(n, A, B);
    }

    event_counter.stop();

    /// Print the result to the console
    const auto result = event_counter.result();

    std::cout << "Sequential(" << n << "): ";
    for (const auto [event_name, value] : result) {
      std::cout << event_name << ": " << value << " ";
    }
    std::cout << std::endl;

    deallocate(A, n);
    deallocate(B, n);
  }

  return 0;
}
