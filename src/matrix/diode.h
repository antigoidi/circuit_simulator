#ifndef DIODE_H_
#define DIODE_H_
#include "device.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

const double Is = 1e-14;
const double N = 1;
const double Vth = 25.9e-3;

class Diode : public Device {
public:
  Diode() = default;
  Diode(double v1, double v2);
  void update(double v1, double v2);
  double getId() const;
  double getGeq() const;
  double getIeq() const;
  void getElementStamp(unsigned int voltage_from, unsigned int voltage_to,
                       std::vector<std::vector<double>> &A,
                       std::vector<double> &b);

  void update(std::vector<double> &voltages) override;
  std::vector<double> getCurrents(std::vector<double> &voltages) override;
  void elementStamp(std::vector<std::vector<double>> &A, std::vector<double> &b,
                    std::vector<int> &node_pos) override;

private:
  double constValue_ = N * Vth;
  double v_ = 0;
  double id_ = 0;
  double Geq_ = 0;
  double Ieq_ = 0;
};

#endif