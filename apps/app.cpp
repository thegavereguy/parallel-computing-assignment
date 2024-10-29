#include <lib/lib.h>

#include <iostream>

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <int> <int>" << std::endl;
    return 1;
  }
  int a = std::stoi(argv[1]);
  int b = std::stoi(argv[2]);

  std::cout << "Sum of " << a << " and " << b << " is " << sum(a, b)
            << std::endl;

  return 0;
}
