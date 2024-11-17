// const int MAT_TRANS_CASES[12] = {2,   4,   8,   16,   32,   64, 128, 256,
// 512, 1024, 2048, 4096};

const int MAT_TRANS_CASES[38] = {
    2,   4,   8,   12,  16,  20,   24,   28,   32,   48,   64,   80,  96,
    112, 128, 144, 160, 176, 192,  208,  224,  240,  256,  288,  320, 352,
    384, 416, 448, 480, 512, 1024, 1536, 2048, 2560, 3072, 3584, 4096};

void transpose_sequential(int n, int **A, int **B);
void transpose_parallel(int n, int **A, int **B);
void transpose_parallel_collapse(int n, int **A, int **B);
