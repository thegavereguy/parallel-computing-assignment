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
      random_allocation(A, n);
      empty_allocation(B, n);
      random_allocation(C, n);
      symmetrize(n, C);

      switch (fun) {
        case 0:
          transpose_sequential(n, A, B);
          break;
        case 1:
          transpose_parallel(n, A, B);
          break;
        case 2:
          transpose_parallel_collapse(n, A, B);
          break;
        case 3:
          transpose_parallel_unroll(n, A, B);
          break;
        case 4:
          transpose_vec(n, A, B);
          break;
        case 5:
          transpose_parallel_sse(n, A, B);
          break;
        case 6:
          symmetry_check_sequential(n, C);
          break;
        case 7:
          symmetry_check_parallel(n, C);
          break;
        case 8:
          symmetry_check_collapse(n, C);
          break;
        case 9:
          symmetry_check_unroll(n, C);
          break;
        case 10:
          symmetry_check_block(n, C);
          break;
      }

      deallocate(A, n);
      deallocate(B, n);
      deallocate(C, n);
    }
  }

  return 0;
}
