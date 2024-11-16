#include <lib/lib.h>
#include <lib/matrix.h>
#include <lib/tools.h>

#include <iostream>

int main(int argc, char** argv) {
  int n   = 100;
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
  deallocate(A, n);
  deallocate(B, n);

  return 0;
}
