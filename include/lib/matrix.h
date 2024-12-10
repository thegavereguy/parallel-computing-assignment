#include <utility>
const int MAT_TRANS_CASES[9] = {16, 32, 64, 128, 256, 512, 1024, 2048, 4096};

// const int MAT_TRANS_CASES[38] = {
//     2,   4,   8,   12,  16,  20,   24,   28,   32,   48,   64,   80,  96,
//     112, 128, 144, 160, 176, 192,  208,  224,  240,  256,  288,  320, 352,
//     384, 416, 448, 480, 512, 1024, 1536, 2048, 2560, 3072, 3584, 4096};
//
std::pair<double, double> transpose_sequential(int n, float **A, float **B);
void transpose_sequential_cont(int n, float *A, float *B);
void transpose_parallel(int n, float **A, float **B);
void transpose_parallel_collapse(int n, float **A, float **B);
void transpose_parallel_unroll(int n, float **A, float **B);
void transpose_parallel_block_cont(int n, float *A, float *B);
void transpose_parallel_block(int n, float **A, float **B);
void transpose_block_sse_single(float *src1, float *src2, float *src3,
                                float *src4, float *dst1, float *dst2,
                                float *dst3, float *dst4);

void transpose_parallel_block_sse(int n, float **A, float **B);
void transpose_avx256(int n, float *A, float *B);
void transpose_block_avx256(float *A, float *B, int n, int, int);

bool symmetry_check_sequential(int n, float **A);
bool symmetry_check_sequential_cont(int n, float *A);
bool symmetry_check_parallel(int n, float **A);
bool symmetry_check_collapse(int n, float **A);
bool symmetry_check_parallel_block(int n, float **A);
bool symmetry_check_parallel_block_cont(int n, float *A);
bool symmetry_check_parallel_unroll(int n, float **A);
// bool symmetry_check_block_sse(int n, float **A);

void symmetrize(int n, float **A);
void symmetrize(int n, float *A);
