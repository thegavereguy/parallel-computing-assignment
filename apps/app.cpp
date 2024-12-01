#include <lib/lib.h>
#include <lib/matrix.h>
#include <lib/tools.h>
#include <perfcpp/event_counter.h>

#include <iostream>

int main(int argc, char** argv) {
  // int n   = 100;
  // int** A = new int*[n];
  // int** B = new int*[n];
  //
  // auto counters      = perf::CounterDefinition();
  // auto event_counter = perf::EventCounter{counters};
  // event_counter.add({"instructions", "cycles", "cache-misses"});
  //
  // for (int i = 0; i < n; i++) {
  //   A[i] = new int[n];
  //   B[i] = new int[n];
  //   for (int j = 0; j < n; j++) {
  //     A[i][j] = i * n + j;
  //   }
  // }
  // event_counter.start();
  //
  // transpose_sequential(n, A, B);
  //
  // event_counter.stop();
  //
  // /// Print the result to the console
  // const auto result = event_counter.result();
  // for (const auto [event_name, value] : result) {
  //   std::cout << event_name << ": " << value << std::endl;
  // }
  // deallocate(A, n);
  // deallocate(B, n);
  // int n     = 4096;
  // float** A = new float*[n];
  // float** B = new float*[n];
  //
  // for (int i = 0; i < n; i++) {
  //   A[i] = new float[n];
  //   B[i] = new float[n];
  //   for (int j = 0; j < n; j++) {
  //     A[i][j] = i * n + j;
  //   }
  // }
  // transpose_parallel_sse(n, A, B);
  // for (int i = 0; i < n; i++) {
  //   for (int j = 0; j < n; j++) {
  //     if (A[i][j] != B[j][i]) {
  //       std::cout << "Error: A[" << i << "][" << j << "] = " << A[i][j]
  //                 << ", B[" << i << "][" << j << "] = " << B[i][j] <<
  //                 std::endl;
  //       return -1;
  //     };
  //     // printf("A[%d][%d] = %f, B[%d][%d] = %f\n", i, j, A[i][j], i, j,
  //     // B[i][j]);
  //   }
  // }
  // deallocate(A, n);
  // deallocate(B, n);
  return 0;
}
