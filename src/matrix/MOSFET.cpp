#include "MOSFET.h"

MOSFET::MOSFET(double vd, double vg, double vs) {
  vds_ = vd - vs;
  vgs_ = vg - vs;
  vov_ = vgs_ - Vt;
  id_ = get_id();
  is_ = get_is();
  ig_ = get_ig();
  Gds_ = get_Gds();
  gm_ = get_gm();
  Ieq_ = get_Ieq();
}

void MOSFET::update(double vd, double vg, double vs) {
  vds_ = vd - vs;
  vgs_ = vg - vs;
  vov_ = vgs_ - Vt;
  id_ = get_id();
  is_ = get_is();
  ig_ = get_ig();
  Gds_ = get_Gds();
  gm_ = get_gm();
  Ieq_ = get_Ieq();
}

double MOSFET::get_id() const {
  if (vov_ > 0) {
    if (vds_ >= 0 && vds_ <= vov_) {
      return Beta * (vov_ * vds_ - 0.5 * std::pow(vds_, 2));
    } else if (vds_ > vov_) {
      return 0.5 * Beta * std::pow(vov_, 2) * (1 + Lambda * vds_);
    }
  }
  return 0;
}

double MOSFET::get_is() const { return -id_; }

double MOSFET::get_ig() const { return 0; }

double MOSFET::get_Gds() const {
  if (vov_ > 0) {
    if (vds_ >= 0 && vds_ <= vov_) {
      return Beta * (vov_ - vds_);
    } else if (vds_ > vov_) {
      return 0.5 * Beta * Lambda * std::pow(vov_, 2);
    }
  }
  return 0;
}

double MOSFET::get_gm() const {
  if (vov_ > 0) {
    if (vds_ >= 0 && vds_ <= vov_) {
      return Beta * vds_;
    } else if (vds_ > vov_) {
      return Beta * vov_ * (1 + Lambda * vds_);
    }
  }
  return 0;
}

double MOSFET::get_Ieq() const { return id_ - Gds_ * vds_ - gm_ * vgs_; }

void MOSFET::getElementStamp(unsigned int d, unsigned int g, unsigned int s,
                             std::vector<std::vector<double>> &A,
                             std::vector<double> &res) {
  A[d][d] += Gds_;
  A[d][s] += -(Gds_ + gm_);
  A[d][g] += gm_;
  A[s][d] += -Gds_;
  A[s][s] += Gds_ + gm_;
  A[s][g] += -gm_;
  // A[g][d] += 0;
  // A[g][s] += 0;
  // A[g][g] += 0;
  res[d] += -Ieq_;
  res[s] += Ieq_;
  // res[g] += 0;
}

void MOSFET::update(std::vector<double> &voltages) {
  assert(voltages.size() == 3);
  update(voltages[0], voltages[1], voltages[2]);
}

std::vector<double> MOSFET::getCurrents(std::vector<double> &voltages) {
  update(voltages);
  return {id_, ig_, is_};
}

void MOSFET::elementStamp(std::vector<std::vector<double>> &A,
                          std::vector<double> &b, std::vector<int> &node_pos) {
  assert(node_pos.size() == 3);
  getElementStamp(node_pos[0], node_pos[1], node_pos[2], A, b);
}