#include <lib/matrix.h>
#include <omp.h>

#include <cstdio>

void transpose_sequential(int n, int **A, int **B) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      B[i][j] = A[j][i];
    }
  }
}

void transpose_parallel(int n, int **A, int **B) {
#pragma omp parallel
  {
#pragma omp for
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        B[i][j] = A[j][i];
      }
    }
  }
}
