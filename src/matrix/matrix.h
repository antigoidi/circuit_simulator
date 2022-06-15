#ifndef MATRIX_H_
#define MATRIX_H_
#include "../parser/parser.h"
#include <Eigen/Dense>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
class Matrix {
public:
  Matrix() = default;
  Matrix(const Parser &parser);
  void voltageStamp(unsigned int voltage_from, unsigned int voltage_to,
                    unsigned int current_idx, double voltage,
                    std::vector<std::vector<double>> &A,
                    std::vector<double> &b);
  void currentStamp1(unsigned int voltage_from, unsigned int voltage_to,
                     unsigned int current_idx, double current,
                     std::vector<std::vector<double>> &A,
                     std::vector<double> &b);
  void currentStamp2(unsigned int voltage_from, unsigned int voltage_to,
                     unsigned int current_idx, double current,
                     std::vector<std::vector<double>> &A,
                     std::vector<double> &b);
  void resistorStamp1(unsigned int voltage_from, unsigned int voltage_to,
                      unsigned int current_idx, double r,
                      std::vector<std::vector<double>> &A,
                      std::vector<double> &b);
  void resistorStamp2(unsigned int voltage_from, unsigned int voltage_to,
                      unsigned int current_idx, double r,
                      std::vector<std::vector<double>> &A,
                      std::vector<double> &b);
  friend std::ostream &operator<<(std::ostream &os, Matrix &mat);

private:
  std::vector<std::vector<double>> A_;
  std::vector<double> b_;
  std::vector<std::string> x_;
  std::vector<double> res_;
  int size_;
};
#endif