#include "BJT.h"

BJT::BJT(double vc, double vb, double ve) {
  vbe_ = vb - ve;
  vbc_ = vb - vc;
  ie_ = get_ie();
  ic_ = get_ic();
  ib_ = get_ib();
  gee_ = get_gee();
  gec_ = get_gec();
  gce_ = get_gec();
  gcc_ = get_gcc();
  Ie_ = get_Ie();
  Ic_ = get_Ic();
}

void BJT::update(double vc, double vb, double ve) {
  vbe_ = vb - ve;
  vbc_ = vb - vc;
  ie_ = get_ie();
  ic_ = get_ic();
  ib_ = get_ib();
  gee_ = get_gee();
  gec_ = get_gec();
  gce_ = get_gec();
  gcc_ = get_gcc();
  Ie_ = get_Ie();
  Ic_ = get_Ic();
}

double BJT::get_ie() const {
  return -Ies * (exp(vbe_ / Vte) - 1) + AR * Ics * (exp(vbc_ / Vtc) - 1);
}

double BJT::get_ic() const {
  return AF * Ies * (exp(vbe_ / Vte) - 1) - Ics * (exp(vbc_ / Vtc) - 1);
}

double BJT::get_ib() const { return -(ie_ + ic_); }

double BJT::get_gee() const { return Ies / Vte * exp(vbe_ / Vte); }

double BJT::get_gec() const { return AR * Ics / Vtc * exp(vbc_ / Vtc); }

double BJT::get_gce() const { return AF * Ies / Vte * exp(vbe_ / Vte); }

double BJT::get_gcc() const { return Ics / Vtc * exp(vbc_ / Vtc); }

double BJT::get_Ie() const { return ie_ + gee_ * vbe_ - gec_ * vbc_; }

double BJT::get_Ic() const { return ic_ - gce_ * vbe_ + gcc_ * vbc_; }

void BJT::getElementStamp(unsigned int c, unsigned int b, unsigned int e,
                          std::vector<std::vector<double>> &A,
                          std::vector<double> &res) {
  A[e][e] += gee_;
  A[e][c] += -gec_;
  A[e][b] += gec_ - gee_;
  A[c][e] += -gce_;
  A[c][c] += gcc_;
  A[c][b] += gce_ - gcc_;
  A[b][e] += gce_ - gee_;
  A[b][c] += gec_ - gcc_;
  A[b][b] += gcc_ + gee_ - gce_ - gec_;
  res[e] += -Ie_;
  res[c] += -Ic_;
  res[b] += Ie_ + Ic_;
}

void BJT::update(std::vector<double> &voltages) {
  assert(voltages.size() == 3);
  update(voltages[0], voltages[1], voltages[2]);
}

std::vector<double> BJT::getCurrents(std::vector<double> &voltages) {
  update(voltages);
  return {ic_, ib_, ie_};
}

void BJT::elementStamp(std::vector<std::vector<double>> &A,
                       std::vector<double> &b, std::vector<int> &node_pos) {
  assert(node_pos.size() == 3);
  getElementStamp(node_pos[0], node_pos[1], node_pos[2], A, b);
}