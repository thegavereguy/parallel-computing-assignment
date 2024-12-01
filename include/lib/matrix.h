const int MAT_TRANS_CASES[9] = {16, 32, 64, 128, 256, 512, 1024, 2048, 4096};

// const int MAT_TRANS_CASES[38] = {
//     2,   4,   8,   12,  16,  20,   24,   28,   32,   48,   64,   80,  96,
//     112, 128, 144, 160, 176, 192,  208,  224,  240,  256,  288,  320, 352,
//     384, 416, 448, 480, 512, 1024, 1536, 2048, 2560, 3072, 3584, 4096};
//
void transpose_sequential(int n, float **A, float **B);
void transpose_parallel(int n, float **A, float **B);
void transpose_parallel_collapse(int n, float **A, float **B);
void transpose_parallel_block(int n, float *A, float *B);
void transpose_parallel_unroll(int n, float **A, float **B);
void transpose_vec(int n, float *A, float *B);
void transpose_block_sse(float *src1, float *dst, int n);
void transpose_parallel_sse(int n, float *A, float *B);

bool symmetry_check_sequential(int n, float **A);
bool symmetry_check_parallel(int n, float **A);
bool symmetry_check_collapse(int n, float **A);
bool symmetry_check_block(int n, float **A);
bool symmetry_check_unroll(int n, float **A);
bool symmetry_check_sse(int n, float **A);
bool symmetry_check_block_sse(int n, float **A);

void symmetrize(int n, float **A);
