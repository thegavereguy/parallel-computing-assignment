#include <lib/tools.h>
void deallocate(int **M, int n) {
  for (int i = 0; i < n; i++) {
    delete[] M[i];
  }
  delete[] M;
}
void deallocate(double **M, int n) {
  for (int i = 0; i < n; i++) {
    delete[] M[i];
  }
  delete[] M;
}
void deallocate(int *M, int n) { delete[] M; }
