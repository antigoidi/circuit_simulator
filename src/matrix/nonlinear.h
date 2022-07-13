#ifndef NONLINEAR_H_
#define NONLINEAR_H_
#include "../parser/parser.h"
#include "BJT.h"
#include "MOSFET.h"
#include "device.h"
#include "diode.h"
#include <memory>
#include <vector>

class Nonlinear {
public:
  Nonlinear() = default;
  void addDevice(Element type, std::vector<int> &node_pos);

  void update(std::vector<double> &node_voltages);
  std::vector<double> getCurrents(std::vector<double> &node_voltages);
  void getStamps(std::vector<std::vector<double>> &A, std::vector<double> &b);

private:
  // record the positions of each node of the elements
  std::vector<std::vector<int>> node_pos_;
  std::vector<std::shared_ptr<Device>> devices_;
  int sz_ = 0;
};

#endif