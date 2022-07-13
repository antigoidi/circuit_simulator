#ifndef MOSFET_H_
#define MOSFET_H_
#include "device.h"
#include <cassert>
#include <cmath>
#include <vector>

const double Vt = 0.6;
const double Lambda = 0.01;
const double Beta = 0.5e-3;

// This is only the nmos
class MOSFET : public Device {
public:
  MOSFET() = default;
  MOSFET(double vd, double vg, double vs);
  void update(double vd, double vg, double vs);
  double get_id() const;
  double get_is() const;
  double get_ig() const;

  double get_Gds() const;
  double get_gm() const;
  double get_Ieq() const;

  void getElementStamp(unsigned int d, unsigned int g, unsigned int s,
                       std::vector<std::vector<double>> &A,
                       std::vector<double> &res);

  void update(std::vector<double> &voltages) override;
  std::vector<double> getCurrents(std::vector<double> &voltages) override;
  void elementStamp(std::vector<std::vector<double>> &A, std::vector<double> &b,
                    std::vector<int> &node_pos) override;

private:
  double vds_ = 0;
  double vgs_ = 0;
  double vov_ = 0;

  double id_ = 0;
  double is_ = 0;
  double ig_ = 0;

  double Gds_ = 0;
  double gm_ = 0;
  double Ieq_ = 0;
};

#endif