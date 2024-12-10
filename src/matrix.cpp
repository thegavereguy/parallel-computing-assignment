#include <immintrin.h>
#include <lib/matrix.h>
#include <omp.h>
#include <xmmintrin.h>

#include <cstdio>
#include <utility>

std::pair<double, double> transpose_sequential(int n, float **A, float **B) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      B[i][j] = A[j][i];
    }
  }
  // return the number of flops and data moved
  return std::make_pair(n * n, n * n * sizeof(float));
}
void transpose_sequential_cont(int n, float *A, float *B) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      B[i * n + j] = A[j * n + i];
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

void transpose_parallel_block(int n, float **A, float **B) {
  int block_size = 4;
  int i, j, ii, jj;
#pragma omp parallel for private(ii, jj)
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

void transpose_parallel_block_cont(int n, float *A, float *B) {
  int block_size = 4;
  int i, j, ii, jj;

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

void transpose_block_sse_single(float *src1, float *src2, float *src3,
                                float *src4, float *dst1, float *dst2,
                                float *dst3, float *dst4) {
  __m128 row1 = _mm_loadu_ps(src1);
  __m128 row2 = _mm_loadu_ps(src2);
  __m128 row3 = _mm_loadu_ps(src3);
  __m128 row4 = _mm_loadu_ps(src4);
  _MM_TRANSPOSE4_PS(row1, row2, row3, row4);

  _mm_storeu_ps(dst1, row1);
  _mm_storeu_ps(dst2, row2);
  _mm_storeu_ps(dst3, row3);
  _mm_storeu_ps(dst4, row4);
}

void transpose_parallel_block_sse(int n, float **A, float **B) {
#pragma omp parallel for
  for (int i = 0; i < n; i += 4) {
    for (int j = 0; j < n; j += 4) {
      transpose_block_sse_single(&A[i][j], &A[i + 1][j], &A[i + 2][j],
                                 &A[i + 3][j], &B[j][i], &B[j + 1][i],
                                 &B[j + 2][i], &B[j + 3][i]);
    }
  }
}
#ifdef __AVX2__
void transpose_block_avx256(float *src, float *dst, int n, int row, int col) {
  // Carica 8 righe consecutive del blocco
  __m256 row0 = _mm256_loadu_ps(&src[(row + 0) * n + col]);
  __m256 row1 = _mm256_loadu_ps(&src[(row + 1) * n + col]);
  __m256 row2 = _mm256_loadu_ps(&src[(row + 2) * n + col]);
  __m256 row3 = _mm256_loadu_ps(&src[(row + 3) * n + col]);
  __m256 row4 = _mm256_loadu_ps(&src[(row + 4) * n + col]);
  __m256 row5 = _mm256_loadu_ps(&src[(row + 5) * n + col]);
  __m256 row6 = _mm256_loadu_ps(&src[(row + 6) * n + col]);
  __m256 row7 = _mm256_loadu_ps(&src[(row + 7) * n + col]);

  // Interleaving: crea colonne trasposte
  __m256 tmp0 =
      _mm256_unpacklo_ps(row0, row1);  // r0[0], r1[0], r0[1], r1[1], ...
  __m256 tmp1 = _mm256_unpackhi_ps(row0, row1);
  __m256 tmp2 = _mm256_unpacklo_ps(row2, row3);
  __m256 tmp3 = _mm256_unpackhi_ps(row2, row3);
  __m256 tmp4 = _mm256_unpacklo_ps(row4, row5);
  __m256 tmp5 = _mm256_unpackhi_ps(row4, row5);
  __m256 tmp6 = _mm256_unpacklo_ps(row6, row7);
  __m256 tmp7 = _mm256_unpackhi_ps(row6, row7);

  // Shuffle: trasponi verticalmente
  __m256 col0 =
      _mm256_shuffle_ps(tmp0, tmp2, 0x44);  // r0[0], r2[0], r1[0], r3[0], ...
  __m256 col1 = _mm256_shuffle_ps(tmp0, tmp2, 0xEE);
  __m256 col2 = _mm256_shuffle_ps(tmp1, tmp3, 0x44);
  __m256 col3 = _mm256_shuffle_ps(tmp1, tmp3, 0xEE);
  __m256 col4 = _mm256_shuffle_ps(tmp4, tmp6, 0x44);
  __m256 col5 = _mm256_shuffle_ps(tmp4, tmp6, 0xEE);
  __m256 col6 = _mm256_shuffle_ps(tmp5, tmp7, 0x44);
  __m256 col7 = _mm256_shuffle_ps(tmp5, tmp7, 0xEE);

  // Store: salva le colonne trasposte nei blocchi
  _mm256_storeu_ps(&dst[(col + 0) * n + row], col0);
  _mm256_storeu_ps(&dst[(col + 1) * n + row], col1);
  _mm256_storeu_ps(&dst[(col + 2) * n + row], col2);
  _mm256_storeu_ps(&dst[(col + 3) * n + row], col3);
  _mm256_storeu_ps(&dst[(col + 4) * n + row], col4);
  _mm256_storeu_ps(&dst[(col + 5) * n + row], col5);
  _mm256_storeu_ps(&dst[(col + 6) * n + row], col6);
  _mm256_storeu_ps(&dst[(col + 7) * n + row], col7);
}
void transpose_avx256(int n, float *A, float *B) {
  int block_size = 8;
  for (int i = 0; i < n; i += block_size) {
    for (int j = 0; j < n; j += block_size) {
      transpose_block_avx256(A, B, n, i, j);
    }
  }
}
#endif

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

bool symmetry_check_sequential_cont(int n, float *A) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (A[i * n + j] != A[j * n + i]) {
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
#pragma omp critical
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
#pragma omp critical
        sym = false;
      }
    }
  }
  return sym;
}

bool symmetry_check_parallel_unroll(int n, float **A) {
  bool sym = true;

#pragma omp parallel for shared(sym)
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j += 4) {
      if (A[i][j] != A[j][i] || A[i][j + 1] != A[j + 1][i] ||
          A[i][j + 2] != A[j + 2][i] || A[i][j + 3] != A[j + 3][i]) {
#pragma omp critical
        sym = false;
      }
    }
  }
  return sym;
}

bool symmetry_check_parallel_block(int n, float **A) {
  bool sym       = true;
  int block_size = 4;

#pragma omp parallel for shared(sym)
  for (int i = 0; i < n; i += block_size) {
    for (int j = 0; j < n; j += block_size) {
      for (int ii = i; ii < i + block_size; ii++) {
        for (int jj = j; jj < j + block_size; jj++) {
          if (A[ii][jj] != A[jj][ii]) {
#pragma omp critical
            sym = false;
          }
        }
      }
    }
  }
  return sym;
}

bool symmetry_check_parallel_block_cont(int n, float *A) {
  bool sym       = true;
  int block_size = 4;

#pragma omp parallel for shared(sym)
  for (int i = 0; i < n; i += block_size) {
    for (int j = 0; j < n; j += block_size) {
      for (int ii = i; ii < i + block_size; ii++) {
        for (int jj = j; jj < j + block_size; jj++) {
          if (A[ii * n + jj] != A[jj * n + ii]) {
#pragma omp critical
            sym = false;
          }
        }
      }
    }
  }
  return sym;
}

// simmtrize the first half of the matrix
void symmetrize(int n, float **A) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
      A[j][i] = A[i][j];
    }
  }
}

void symmetrize(int n, float *A) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
      A[j * n + i] = A[i * n + j];
    }
  }
}
