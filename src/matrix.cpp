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
    for (int j = 0; j < n - 4; j += 4) {
      B[i][j]     = A[j][i];
      B[i][j + 1] = A[j + 1][i];
      B[i][j + 2] = A[j + 2][i];
      B[i][j + 3] = A[j + 3][i];
    }
  }
}

// matrix transposition with vectorization
void transpose_vec(int n, int **A, int **B) {
#pragma omp simd
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      B[i][j] = A[j][i];
    }
  }
}

void transpose_parallel_block(int n, int **A, int **B) {
  int block_size = 4;
  int i, j, ii, jj;
  for (i = 0; i < n; i += block_size) {
    for (j = 0; j < n; j += block_size) {
      for (ii = i; ii < i + block_size; ii++) {
        for (jj = j; jj < j + block_size; jj++) {
          B[ii][jj] = A[jj][ii];
        }
      }
    }
  }
}

void transpose_parallel_row(int n, int **A, int **B) {
  for (int i = 0; i < n; i++) {
    for (int k = 0; k < n; k++) {
      B[i][k] = A[k][i];
    }
  }
}
