#ifndef MATRIX_H_
#define MATRIX_H_
#include "../parser/parser.h"
#include "nonlinear.h"
#include <Eigen/Dense>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

const double K = 16;
const double Gamma = 1.3;

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
  int constructH(TEdgeMap &tmap, std::vector<std::vector<double>> &H,
                 std::vector<std::string> &h_str, int k);
  std::vector<double> getNonlinearCurrents(std::vector<double> &voltages);

  void nonlinearStamps(std::vector<std::vector<double>> &A,
                       std::vector<double> &b);
  friend std::ostream &operator<<(std::ostream &os, Matrix &mat);

  void update();
  std::vector<double> getNodeVoltages();

  Eigen::MatrixXd matrixConvert(std::vector<std::vector<double>> &vec2d,
                                int sz1, int sz2);
  Eigen::VectorXd vectorConvert(std::vector<double> &vec, int sz);
  void getSysEqns();

  double damping(double num_old, double num_new);
  void dampMethod();
  bool isLinear() const;
  double getNorm();

private:
  std::vector<std::vector<double>> G_;
  std::vector<double> b_ori_;

  std::vector<std::vector<double>> A_;
  std::vector<double> b_;
  std::vector<std::string> x_;
  std::vector<double> res_;
  std::vector<double> res_old_;
  int size_;

  bool linear_;
  std::vector<std::vector<double>> H_;
  std::vector<std::string> h_str_;
  Nonlinear nonlinear_;
  int node_count_;
};
#endif