#include "nonlinear.h"

void Nonlinear::addDevice(Element type, std::vector<int> &node_pos) {
  switch (type) {
  case D:
    devices_.push_back(std::make_shared<Diode>());
    node_pos_.push_back(node_pos);
    sz_++;
    break;
  case QN:
    devices_.push_back(std::make_shared<BJT>());
    node_pos_.push_back(node_pos);
    sz_++;
    break;
  case MN:
    devices_.push_back(std::make_shared<MOSFET>());
    node_pos_.push_back(node_pos);
    sz_++;
    break;
  default:
    break;
  }
}

void Nonlinear::update(std::vector<double> &node_voltages) {
  for (int i = 0; i < sz_; i++) {
    std::vector<double> voltages;
    for (int j = 0; j < node_pos_[i].size(); j++) {
      voltages.push_back(node_voltages[node_pos_[i][j]]);
    }
    devices_[i]->update(voltages);
  }
}

std::vector<double> Nonlinear::getCurrents(std::vector<double> &node_voltages) {
  std::vector<double> res;
  for (int i = 0; i < sz_; i++) {
    std::vector<double> voltages;
    for (int j = 0; j < node_pos_[i].size(); j++) {
      voltages.push_back(node_voltages[node_pos_[i][j]]);
    }
    auto vec = devices_[i]->getCurrents(voltages);
    res.insert(res.begin(), vec.begin(), vec.end());
  }
  return res;
}

void Nonlinear::getStamps(std::vector<std::vector<double>> &A,
                          std::vector<double> &b) {
  for (int i = 0; i < sz_; i++) {
    devices_[i]->elementStamp(A, b, node_pos_[i]);
  }
}
