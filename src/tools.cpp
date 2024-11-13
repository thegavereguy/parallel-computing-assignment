#include <lib/tools.h>

#include <cstdlib>

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

void random_allocation(int **M, int n) {
  int **A = new int *[n];
  for (int i = 0; i < n; i++) {
    A[i] = new int[n]();
    for (int j = 0; j < n; j++) {
      A[i][j] = rand() % n;
    }
  }
}
