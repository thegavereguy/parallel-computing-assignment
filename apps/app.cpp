#include <lib/lib.h>

#include <iostream>

int main() {
  // initialize a random variable and print it
  int x = 5;
  x     = x + 1;
  x     = sum(2, 54);
  std::cout << "value of variable: " << x << std::endl;
  return 0;
}
