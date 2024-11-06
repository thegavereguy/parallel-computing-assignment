#include <lib/lib.h>
#include <lib/matrix.h>
#include <lib/tools.h>

#include <iostream>

int main(int argc, char** argv) {
  int a = 2;
  int b = 4;

  std::cout << "Sum of " << a << " and " << b << " is " << sum(a, b)
            << std::endl;
  std::cout << "Product of " << a << " and " << b << " is " << mul(a, b)
            << std::endl;

  parallel_test();
  std::cout << "Pi: " << calculate_pi_parallel(10000000) << std::endl;

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
      std::cout << B[i][j] << " ";
    }
  }
  deallocate(A, n);
  deallocate(B, n);

  return 0;
}
