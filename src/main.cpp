#include "matrix/matrix.h"
#include "parser/parser.h"
#include <fstream>
#include <iostream>
#include <string>
int main(int argc, char **argv) {
  std::cout << "Hi, this is a circuit simulator\n";
  if (argc != 2) {
    std::cout << "usage: ./circuit_simulator <file>\n";
    return 0;
  }
  std::ifstream MyReadFile(argv[1]);
  Parser parser;
  if (parser.readFile(MyReadFile)) {
    // do something
    std::cout << parser;
    Matrix matrix(parser);
    std::cout << matrix;
  }
  return 0;
}