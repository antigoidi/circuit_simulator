#include "diode.h"

Diode::Diode(double v1, double v2) {
  v_ = v1 - v2;
  id_ = getId();
  Geq_ = getGeq();
  Ieq_ = getIeq();
}

void Diode::update(double v1, double v2) {
  v_ = v1 - v2;
  id_ = getId();
  Geq_ = getGeq();
  Ieq_ = getIeq();
}

double Diode::getId() const { return Is * (std::exp(v_ / constValue_) - 1); }

double Diode::getGeq() const {
  return Is / constValue_ * std::exp(v_ / constValue_);
}

double Diode::getIeq() const { return id_ - Geq_ * v_; }
void Diode::getElementStamp(unsigned int voltage_from, unsigned int voltage_to,
                            std::vector<std::vector<double>> &A,
                            std::vector<double> &b) {
  A[voltage_from][voltage_from] += Geq_;
  A[voltage_from][voltage_to] += -Geq_;
  A[voltage_to][voltage_from] += -Geq_;
  A[voltage_to][voltage_to] += Geq_;
  b[voltage_from] += -Ieq_;
  b[voltage_to] += Ieq_;
}

void Diode::update(std::vector<double> &voltages) {
  assert(voltages.size() == 2);
  update(voltages[0], voltages[1]);
}

std::vector<double> Diode::getCurrents(std::vector<double> &voltages) {
  update(voltages);
  return {id_};
}

void Diode::elementStamp(std::vector<std::vector<double>> &A,
                         std::vector<double> &b, std::vector<int> &node_pos) {
  assert(node_pos.size() == 2);
  getElementStamp(node_pos[0], node_pos[1], A, b);
}