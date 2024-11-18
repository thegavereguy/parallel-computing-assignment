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
  for (int i = 0; i < n; i++) {
    M[i] = new int[n]();
    for (int j = 0; j < n; j++) {
      M[i][j] = rand() % n;
    }
  }
}

void empty_allocation(int **M, int n) {
  for (int i = 0; i < n; i++) {
    M[i] = new int[n]();
  }
}
