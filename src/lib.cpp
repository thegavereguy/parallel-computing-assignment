#include <lib/lib.h>
#include <omp.h>

#include <iostream>

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

void parallel_test() {
  int shared_var = 0;

#pragma omp parallel num_threads(4)
  {
    int tid = omp_get_thread_num();
    // std::cout << "Thread " << tid << " is running" << std::endl;
    printf("Thread %d is running\n", tid);
    shared_var += tid;
  }
  std::cout << "Shared var: " << shared_var << std::endl;
}

float calculate_pi(double n) {
  int i;
  double x, sum = 0.0, pi = 0.0;
  double step = 1.0 / (double)n;
  for (i = 0; i < n; i++) {
    x   = (i + 0.5) * step;
    sum = sum + 4.0 / (1.0 + x * x);
  }
  return sum;
}

float calculate_pi_parallel(double n) {
  int i;
  double x, sum = 0.0, pi = 0.0;
  double step = 1.0 / (double)n;

  struct timespec start, end;

  clock_gettime(CLOCK_MONOTONIC, &start);

#pragma omp parallel
  {
    for (i = 0; i < n; i++) {
      x   = (i + 0.5) * step;
      sum = sum + 4.0 / (1.0 + x * x);
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &end);

  long seconds_ts              = end.tv_sec - start.tv_sec;
  long nanoseconds_ts          = end.tv_nsec - start.tv_nsec;
  double elapsed_clock_gettime = seconds_ts + nanoseconds_ts * 1e-9;

  std::cout << "Elapsed time using clock_gettime: " << elapsed_clock_gettime
            << " seconds" << std::endl;

  return sum;
}

double array_sum_sequential(int *v, int n) {
  double sum = 0;
  for (int i = 0; i < n; i++) sum += v[i];
  return sum;
}

double array_sum_parallel(int *v, int n) {
  double sum = 0;

#pragma omp parallel for reduction(+ : sum) num_threads(4)
  for (int i = 0; i < n; i++) {
    sum += v[i];
  }

  return sum;
}
