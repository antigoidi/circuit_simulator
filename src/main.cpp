#include <iostream>
#include "parser/adder.h"
int main() {
  std::cout << "Hi, this is a circuit simulator\n";
  std::cout << "75.23 + 43.19 = " << add(75.23f, 43.19f) << std::endl;
  return 0;
}