#include <lib/tools.h>

#include <cmath>
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

bool check_transpose(int **A, int **B, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (A[i][j] != B[j][i]) {
        return false;
      }
    }
  }
  return true;
}

Results::Results() {
  instructions = std::vector<double>();
  cycles       = std::vector<double>();
  cache_misses = std::vector<double>();
}

void Results::push_back_cycles(double value) { cycles.push_back(value); }
void Results::push_back_instructions(double value) {
  instructions.push_back(value);
}
void Results::push_back_cache_misses(double value) {
  cache_misses.push_back(value);
}
double Results::mean_cycles(double conf) { return mean(cycles, conf); }
double Results::mean_instructions(double conf) {
  return mean(instructions, conf);
}
double Results::mean_cache_misses(double conf) {
  return mean(cache_misses, conf);
}
double Results::mean(std::vector<double> &v) {
  double sum = 0;
  for (auto &i : v) {
    sum += i;
  }
  return sum / v.size();
}
double Results::std_dev(std::vector<double> &v) {
  double m   = mean(v);
  double sum = 0;
  for (auto &i : v) {
    sum += (i - m) * (i - m);
  }
  return sum / v.size();
}

double Results::mean(std::vector<double> &v, double conf) {
  double m = mean(v);
  double s = std_dev(v);
  conf     = 1.96 * s / sqrt(v.size());
  return m;
}
