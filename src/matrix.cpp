#include <immintrin.h>
#include <lib/matrix.h>
#include <omp.h>

#include <cstdio>

void transpose_sequential(int n, float **A, float **B) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      B[i][j] = A[j][i];
    }
  }
}

void transpose_parallel(int n, float **A, float **B) {
#pragma omp parallel for
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      B[i][j] = A[j][i];
    }
  }
}

void transpose_parallel_collapse(int n, float **A, float **B) {
#pragma omp parallel for collapse(2)
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      B[i][j] = A[j][i];
    }
  }
}

// matrix transposition with loop unroll
void transpose_parallel_unroll(int n, float **A, float **B) {
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

// matrix transposition with vectorization
void transpose_vec(int n, float *A, float *B) {
#pragma omp simd
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      B[i * n + j] = A[j * n + i];
    }
  }
}

void transpose_parallel_block(int n, float *A, float *B) {
  int block_size = 4;  // Block size of 4
  int i, j, ii, jj;

// Parallelize the outer loops
#pragma omp parallel for private(ii, jj)
  for (i = 0; i < n; i += block_size) {
    for (j = 0; j < n; j += block_size) {
      for (ii = i; ii < i + block_size; ii++) {
        for (jj = j; jj < j + block_size; jj++) {
          B[ii * n + jj] = A[jj * n + ii];
        }
      }
    }
  }
}

void transpose_parallel_row(int n, float **A, float **B) {
  for (int i = 0; i < n; i++) {
    for (int k = 0; k < n; k++) {
      B[i][k] = A[k][i];
    }
  }
}

void transpose_block_sse(float *src, float *dst, int n) {
  __m128 row1 = _mm_loadu_ps(&src[0 * n]);
  __m128 row2 = _mm_loadu_ps(&src[1 * n]);
  __m128 row3 = _mm_loadu_ps(&src[2 * n]);
  __m128 row4 = _mm_loadu_ps(&src[3 * n]);

  _MM_TRANSPOSE4_PS(row1, row2, row3, row4);

  _mm_storeu_ps(&dst[0 * n], row1);
  _mm_storeu_ps(&dst[1 * n], row2);
  _mm_storeu_ps(&dst[2 * n], row3);
  _mm_storeu_ps(&dst[3 * n], row4);
}

void transpose_parallel_sse(int n, float *A, float *B) {
#pragma omp parallel for
  for (int i = 0; i < n; i += 4) {
    for (int j = 0; j < n; j += 4) {
      // Transpose a 4x4 block
      transpose_block_sse(&A[i * n + j], &B[j * n + i], n);
    }
  }
}

bool symmetry_check_sequential(int n, float **A) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (A[i][j] != A[j][i]) {
        return false;
      }
    }
  }
  return true;
}

bool symmetry_check_parallel(int n, float **A) {
  bool sym = true;
#pragma omp parallel for shared(sym)
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (A[i][j] != A[j][i]) {
        sym = false;
      }
    }
  }
  return sym;
}

bool symmetry_check_collapse(int n, float **A) {
  bool sym = true;
#pragma omp parallel for collapse(2) shared(sym)
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (A[i][j] != A[j][i]) {
        sym = false;
      }
    }
  }
  return sym;
}

bool symmetry_check_unroll(int n, float **A) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j += 4) {
      if (A[i][j] != A[j][i] || A[i][j + 1] != A[j + 1][i] ||
          A[i][j + 2] != A[j + 2][i] || A[i][j + 3] != A[j + 3][i]) {
        return false;
      }
    }
  }
  return true;
}

bool symmetry_check_block(int n, float **A) {
  for (int i = 0; i < n; i += 4) {
    for (int j = 0; j < n; j += 4) {
      for (int ii = i; ii < i + 4; ii++) {
        for (int jj = j; jj < j + 4; jj++) {
          if (A[ii][jj] != A[jj][ii]) {
            return false;
          }
        }
      }
    }
  }
  return true;
}

// simmtrize the first half of the matrix
void symmetrize(int n, float **A) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
      A[j][i] = A[i][j];
    }
  }
}
