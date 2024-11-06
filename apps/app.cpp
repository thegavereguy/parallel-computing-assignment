#include <lib/lib.h>

#include <iostream>

int main(int argc, char** argv) {
  int a = 2;
  int b = 4;

  std::cout << "Sum of " << a << " and " << b << " is " << sum(a, b)
            << std::endl;
  std::cout << "Product of " << a << " and " << b << " is " << mul(a, b)
            << std::endl;

  parallel_test();

  return 0;
}
