#include <lib/lib.h>
#include <lib/matrix.h>
#include <lib/tools.h>
#include <perfcpp/event_counter.h>

#include <iostream>

int main(int argc, char** argv) {
  int n   = 100;
  int** A = new int*[n];
  int** B = new int*[n];

  auto counters      = perf::CounterDefinition();
  auto event_counter = perf::EventCounter{counters};
  event_counter.add({"instructions", "cycles", "cache-misses"});

  for (int i = 0; i < n; i++) {
    A[i] = new int[n];
    B[i] = new int[n];
    for (int j = 0; j < n; j++) {
      A[i][j] = i * n + j;
    }
  }
  event_counter.start();

  transpose_sequential(n, A, B);

  event_counter.stop();

  /// Print the result to the console
  const auto result = event_counter.result();
  for (const auto [event_name, value] : result) {
    std::cout << event_name << ": " << value << std::endl;
  }
  deallocate(A, n);
  deallocate(B, n);

  return 0;
}
