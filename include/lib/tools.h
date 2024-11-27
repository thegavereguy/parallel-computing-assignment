#include <vector>
void deallocate(int **, int);
void deallocate(double **, int);
void deallocate(int *, int);
void random_allocation(int **, int);
void empty_allocation(int **, int);
bool check_transpose(int **, int **, int);

class Results {
 private:
  std::vector<double> instructions;
  std::vector<double> cycles;
  std::vector<double> cache_misses;
  // calculate means and standard deviations
  double mean(std::vector<double> &);
  double std_dev(std::vector<double> &);
  // mean, standard deviation, and confidence interval
  double mean(std::vector<double> &, double);

 public:
  // methods to push back values
  void push_back_instructions(double);
  void push_back_cycles(double);
  void push_back_cache_misses(double);

  double mean_instructions(double);
  double mean_cycles(double);
  double mean_cache_misses(double);

  Results();
};
