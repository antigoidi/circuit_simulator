#include "matrix/matrix.h"
#include "parser/parser.h"
#include <Eigen/Dense>
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
    if (!matrix.isLinear()) {
      matrix.dampMethod();
      while (matrix.getNorm() > 1e-5) {
        matrix.update();
        // std::cout.precision(10);
        matrix.dampMethod();
        std::cout << matrix;
      }
      matrix.getSysEqns();
    }
  }
  return 0;
}