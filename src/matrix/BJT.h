#ifndef BJT_H_
#define BJT_H_
#include "device.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

const double AF = 0.99;
const double AR = 0.02;
const double Ies = 2e-14;
const double Ics = 99e-14;
const double Vtc = 26e-3;
const double Vte = 26e-3;

// This is only the NPN diode
class BJT : public Device {
public:
  BJT() = default;
  BJT(double vc, double vb, double ve);
  void update(double vc, double vb, double ve);
  double get_ie() const;
  double get_ic() const;
  double get_ib() const;

  double get_gee() const;
  double get_gec() const;
  double get_gce() const;
  double get_gcc() const;

  double get_Ie() const;
  double get_Ic() const;

  void getElementStamp(unsigned int c, unsigned int b, unsigned int e,
                       std::vector<std::vector<double>> &A,
                       std::vector<double> &res);

  void update(std::vector<double> &voltages) override;
  std::vector<double> getCurrents(std::vector<double> &voltages) override;
  void elementStamp(std::vector<std::vector<double>> &A, std::vector<double> &b,
                    std::vector<int> &node_pos) override;

private:
  double vbe_ = 0;
  double vbc_ = 0;
  double ie_ = 0;
  double ic_ = 0;
  double ib_ = 0;
  double gee_ = 0;
  double gec_ = 0;
  double gce_ = 0;
  double gcc_ = 0;
  double Ie_ = 0;
  double Ic_ = 0;
};

#endif