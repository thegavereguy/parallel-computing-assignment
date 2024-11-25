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
#pragma omp parallel for
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      B[i][j] = A[j][i];
    }
  }
}

void transpose_parallel_collapse(int n, int **A, int **B) {
#pragma omp parallel for collapse(2)
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      B[i][j] = A[j][i];
    }
  }
}

// matrix transposition with loop unroll
void transpose_parallel_unroll(int n, int **A, int **B) {
#pragma omp parallel for
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j += 4) {
      B[i][j]     = A[j][i];
      B[i][j + 1] = A[j + 1][i];
      B[i][j + 2] = A[j + 2][i];
      B[i][j + 3] = A[j + 3][i];
    }
  }
}
