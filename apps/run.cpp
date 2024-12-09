#include <lib/lib.h>
#include <lib/matrix.h>
#include <lib/tools.h>

#include <iostream>

int main(int argc, char **argv) {
  if (argc != 3 || std::string(argv[1]) == "help") {
    std::cerr << "Usage: " << argv[0] << " <samples> <function>" << std::endl;
    std::cerr << "Function: 0=sequential, 1=parallel, 2=parallel_collapse"
              << std::endl;
    return 1;
  }

  int samples = std::stoi(argv[1]);
  int fun     = std::stoi(argv[2]);

  //  std::cout << "DIMENSION,INST,CYCLES,CACHE-MISSES" << std::endl;
  for (int n : MAT_TRANS_CASES) {
    Results res;
    for (int i = 0; i < samples; i++) {
      float **A = new float *[n];
      float **B = new float *[n];
      float **C = new float *[n];
      float *D  = new float[n * n];
      float *E  = new float[n * n];
      random_allocation(A, n);
      empty_allocation(B, n);
      random_allocation(C, n);
      random_allocation_contiguous(D, n);
      symmetrize(n, C);
      symmetrize(n, E);

      switch (fun) {
        case 0:
          transpose_sequential(n, A, B);
          break;
        case 1:
          transpose_sequential_cont(n, D, E);
        case 2:
          transpose_parallel(n, A, B);
          break;
        case 3:
          transpose_parallel_collapse(n, A, B);
          break;
        case 4:
          transpose_parallel_unroll(n, A, B);
          break;
        case 5:
          transpose_parallel_block(n, A, B);
          break;
        case 6:
          transpose_parallel_block_cont(n, D, E);
          break;
        case 7:
          transpose_parallel_block_sse(n, A, B);
          break;
        case 8:
          symmetry_check_sequential(n, C);
          break;
        case 9:
          symmetry_check_sequential_cont(n, E);
          break;
        case 10:
          symmetry_check_parallel(n, C);
          break;
        case 11:
          symmetry_check_collapse(n, C);
          break;
        case 12:
          symmetry_check_parallel_unroll(n, C);
          break;
        case 13:
          symmetry_check_parallel_block(n, C);
          break;
        case 14:
          symmetry_check_parallel_block_cont(n, E);
      }

      deallocate(A, n);
      deallocate(B, n);
      deallocate(C, n);
      delete[] D;
      delete[] E;
    }
  }

  return 0;
}
