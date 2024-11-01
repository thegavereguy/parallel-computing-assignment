#include <lib/lib.h>
int sum(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }

void merge(double *arr, int l, int m, int r) {
  int n1 = m - l + 1;
  int n2 = r - m;
  double L[n1], R[n2];
  for (int i = 0; i < n1; i++) L[i] = arr[l + i];
  for (int i = 0; i < n2; i++) R[i] = arr[m + 1 + i];
  int i = 0, j = 0, k = l;
  while (i < n1 && j < n2) {
    if (L[i] <= R[j])
      arr[k] = L[i++];
    else
      arr[k] = R[j++];
    k++;
  }
  while (i < n1) arr[k++] = L[i++];
  while (j < n2) arr[k++] = R[j++];
}
void merge_sort(double *arr, int l, int r) {
  if (l < r) {
    int m = l + (r - l) / 2;
    merge_sort(arr, l, m);
    merge_sort(arr, m + 1, r);
    merge(arr, l, m, r);
  }
}
