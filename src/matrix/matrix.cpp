#include "matrix.h"

Matrix::Matrix(const Parser &parser) {
  std::cout << "Start building matrix\n";
  linear_ = parser.isLinear();
  EdgeMap v_map = parser.getEdges(V);
  EdgeMap i_map = parser.getEdges(I);
  EdgeMap r_map = parser.getEdges(R);
  node_count_ = parser.getNodeCount();
  std::cout << "Node count is " << node_count_ << std::endl;
  int sz = node_count_ + 1 + parser.getCurrentEdgesSize();
  size_ = sz - 1;
  std::cout << "Size is " << sz << std::endl;
  std::vector<std::vector<double>> A(sz, std::vector<double>(sz, 0));
  std::vector<double> b(sz, 0);
  x_.resize(size_);
  for (int i = 1; i <= node_count_; i++) {
    x_[i - 1] = "V(" + std::to_string(i) + ")";
  }
  for (auto it = v_map.begin(); it != v_map.end(); ++it) {
    auto current_idx =
        node_count_ + 1 +
        parser.getCurrentEdgeIndex(it->second.from_, it->second.to_);
    x_[current_idx - 1] = "I(V" + std::to_string(it->second.index_) + ")";
    voltageStamp(it->second.from_, it->second.to_, current_idx,
                 it->second.value_, A, b);
  }
  for (auto it = i_map.begin(); it != i_map.end(); ++it) {
    auto current_idx =
        node_count_ + 1 +
        parser.getCurrentEdgeIndex(it->second.from_, it->second.to_);
    if (it->second.option_ == "G2") {
      currentStamp2(it->second.from_, it->second.to_, current_idx,
                    it->second.value_, A, b);
      x_[current_idx - 1] = "I(I" + std::to_string(it->second.index_) + ")";
    } else {
      currentStamp1(it->second.from_, it->second.to_, current_idx,
                    it->second.value_, A, b);
    }
  }
  for (auto it = r_map.begin(); it != r_map.end(); ++it) {
    auto current_idx =
        node_count_ + 1 +
        parser.getCurrentEdgeIndex(it->second.from_, it->second.to_);
    if (it->second.option_ == "G2") {
      resistorStamp2(it->second.from_, it->second.to_, current_idx,
                     it->second.value_, A, b);
      x_[current_idx - 1] = "I(R" + std::to_string(it->second.index_) + ")";
    } else {
      resistorStamp1(it->second.from_, it->second.to_, current_idx,
                     it->second.value_, A, b);
    }
  }
  G_ = A;
  b_ori_ = b;
  if (!linear_) {
    std::cout << "Start building nonlinear matrix\n";
    EdgeMap d_map = parser.getEdges(D);
    TEdgeMap qn_map = parser.getTEdges(QN);
    TEdgeMap qp_map = parser.getTEdges(QP);
    TEdgeMap mn_map = parser.getTEdges(MN);
    TEdgeMap mp_map = parser.getTEdges(MP);
    int NLECount = parser.getNLECount();
    std::vector<std::vector<double>> H(size_, std::vector<double>(NLECount, 0));
    h_str_.resize(NLECount);
    int i = 0;
    for (auto it = d_map.begin(); it != d_map.end(); it++) {
      h_str_[i] =
          convert(it->second.type_) + std::to_string(it->second.index_) + "_i";
      int from = it->second.from_;
      int to = it->second.to_;
      std::cout << "debug 1\n";
      if (from > 0) {
        ++H[from - 1][i];
      }
      if (to > 0) {
        --H[to - 1][i];
      }
      std::vector<int> temp = {from, to};
      nonlinear_.addDevice(D, temp);
      ++i;
    }
    for (auto map : {qn_map, qp_map, mn_map, mp_map}) {
      constructH(map, H, h_str_, i);
    }
    H_ = H;

    nonlinearStamps(A, b);
  }

  std::cout << "Building matrix done\n";
  A_.clear();
  b_.clear();
  res_old_.clear();
  std::vector<double> vec(size_, 0.0);
  res_old_ = std::move(vec);
  Eigen::MatrixXd A1(size_, size_);
  Eigen::VectorXd b1(size_);
  for (int i = 1; i <= size_; i++) {
    std::vector<double> temp;
    for (int j = 1; j <= size_; j++) {
      temp.push_back(A[i][j]);
      A1(i - 1, j - 1) = A[i][j];
    }
    A_.push_back(temp);
    b_.push_back(b[i]);
    b1(i - 1) = b[i];
  }
  Eigen::VectorXd x = A1.lu().solve(b1);
  for (auto num : x) {
    res_.push_back(num);
  }
}

void Matrix::voltageStamp(unsigned int voltage_from, unsigned int voltage_to,
                          unsigned int current_idx, double voltage,
                          std::vector<std::vector<double>> &A,
                          std::vector<double> &b) {
  A[voltage_from][current_idx] += 1;
  A[voltage_to][current_idx] -= 1;
  A[current_idx][voltage_from] += 1;
  A[current_idx][voltage_to] -= 1;
  b[current_idx] += voltage;
}

void Matrix::currentStamp1(unsigned int voltage_from, unsigned int voltage_to,
                           unsigned int current_idx, double current,
                           std::vector<std::vector<double>> &A,
                           std::vector<double> &b) {
  b[voltage_from] -= current;
  b[voltage_to] += current;
}

void Matrix::currentStamp2(unsigned int voltage_from, unsigned int voltage_to,
                           unsigned int current_idx, double current,
                           std::vector<std::vector<double>> &A,
                           std::vector<double> &b) {
  A[voltage_from][current_idx] += 1;
  A[voltage_to][current_idx] -= 1;
  A[current_idx][current_idx] += 1;
  b[current_idx] += current;
}

void Matrix::resistorStamp1(unsigned int voltage_from, unsigned int voltage_to,
                            unsigned int current_idx, double r,
                            std::vector<std::vector<double>> &A,
                            std::vector<double> &b) {
  A[voltage_from][voltage_from] += 1 / r;
  A[voltage_from][voltage_to] -= 1 / r;
  A[voltage_to][voltage_from] -= 1 / r;
  A[voltage_to][voltage_to] += 1 / r;
}

void Matrix::resistorStamp2(unsigned int voltage_from, unsigned int voltage_to,
                            unsigned int current_idx, double r,
                            std::vector<std::vector<double>> &A,
                            std::vector<double> &b) {
  A[voltage_from][current_idx] += 1;
  A[voltage_to][current_idx] -= 1;
  A[current_idx][voltage_from] += 1;
  A[current_idx][voltage_to] -= 1;
  A[current_idx][current_idx] -= r;
}

int Matrix::constructH(TEdgeMap &tmap, std::vector<std::vector<double>> &H,
                       std::vector<std::string> &h_str, int k) {
  int res = k;
  if (!tmap.empty()) {
    for (auto it = tmap.begin(); it != tmap.end(); it++) {
      int from = it->second.from_ - 1;
      int mid = it->second.mid_ - 1;
      int to = it->second.to_ - 1;
      if (from >= 0) {
        ++H[from][k];
      }
      h_str_[k] =
          convert(it->second.type_) + std::to_string(it->second.index_) + "_ic";
      ++k;
      if (mid >= 0) {
        ++H[mid][k];
      }
      h_str_[k] =
          convert(it->second.type_) + std::to_string(it->second.index_) + "_ib";
      ++k;
      if (to >= 0) {
        ++H[to][k];
      }
      h_str_[k] =
          convert(it->second.type_) + std::to_string(it->second.index_) + "_ie";
      ++k;
      std::vector<int> temp = {from + 1, mid + 1, to + 1};
      nonlinear_.addDevice(it->second.type_, temp);
    }
  }
  return res;
}

std::vector<double>
Matrix::getNonlinearCurrents(std::vector<double> &node_voltages) {
  return nonlinear_.getCurrents(node_voltages);
}

void Matrix::nonlinearStamps(std::vector<std::vector<double>> &A,
                             std::vector<double> &b) {
  nonlinear_.getStamps(A, b);
}

std::ostream &operator<<(std::ostream &os, Matrix &mat) {
  os << "Ax = b:\n";
  for (int i = 0; i < mat.size_; i++) {
    for (int j = 0; j < mat.size_; j++) {
      os << mat.A_[i][j] << " ";
    }
    os << " | ";
    os << mat.x_[i];
    os << " | ";
    os << mat.res_[i];
    os << " | ";
    os << mat.b_[i] << "\n";
  }
  os << "\n";

  os << "Answer: \n";
  for (int i = 0; i < mat.size_; i++) {
    os << mat.x_[i];
    os << " = ";
    os << mat.res_[i] << "\n";
  }
  os << "\n";

  if (!mat.linear_) {
    // os << "H: \n";
    // for (auto v : mat.H_) {
    //   for(auto num : v) {
    //     os << num << " ";
    //   }
    //   os << "\n";
    // }
    // os << "\n";

    // os << "h_str: \n";
    // for(auto str: mat.h_str_) {
    //   os << str << "\n";
    // }
    // os << "\n";
    auto vec = mat.getNodeVoltages();
    auto temp = mat.getNonlinearCurrents(vec);
    os << "nonlinear currents: \n";
    for (auto current : temp) {
      os << current << "\n";
    }
    os << "\n";
  }
  os << "~~~~~~~~~~~~~~~~~~~~~~\n";
  return os;
}

void Matrix::update() {
  std::vector<double> node_voltages = getNodeVoltages();
  nonlinear_.update(node_voltages);
  std::vector<std::vector<double>> A = G_;
  std::vector<double> b = b_ori_;
  nonlinear_.getStamps(A, b);

  A_.clear();
  b_.clear();
  res_.clear();
  Eigen::MatrixXd A1(size_, size_);
  Eigen::VectorXd b1(size_);
  for (int i = 1; i <= size_; i++) {
    std::vector<double> temp;
    for (int j = 1; j <= size_; j++) {
      temp.push_back(A[i][j]);
      A1(i - 1, j - 1) = A[i][j];
    }
    A_.push_back(temp);
    b_.push_back(b[i]);
    b1(i - 1) = b[i];
  }
  Eigen::VectorXd x = A1.lu().solve(b1);
  for (auto num : x) {
    res_.push_back(num);
  }
}

std::vector<double> Matrix::getNodeVoltages() {
  std::vector<double> ret{0.0};
  ret.insert(ret.end(), res_.begin(), res_.begin() + node_count_);
  return ret;
}

Eigen::MatrixXd Matrix::matrixConvert(std::vector<std::vector<double>> &vec2d,
                                      int sz1, int sz2) {
  Eigen::MatrixXd mat(sz1, sz2);
  for (int i = 0; i < sz1; i++) {
    for (int j = 0; j < sz2; j++) {
      mat(i, j) = vec2d[i][j];
    }
  }
  return mat;
}

Eigen::VectorXd Matrix::vectorConvert(std::vector<double> &vec, int sz) {
  Eigen::VectorXd v(sz);
  for (int i = 0; i < sz; i++) {
    v(i) = vec[i];
  }
  return v;
}

void Matrix::getSysEqns() {
  std::vector<std::vector<double>> temp_G;
  std::vector<double> temp_b;
  for (int i = 1; i <= size_; i++) {
    std::vector<double> temp;
    for (int j = 1; j <= size_; j++) {
      temp.push_back(G_[i][j]);
    }
    temp_G.push_back(temp);
    temp_b.push_back(b_ori_[i]);
  }
  Eigen::MatrixXd G = matrixConvert(temp_G, size_, size_);
  // std::cout.precision(10);
  std::cout << "G: \n" << G << "\n";
  Eigen::VectorXd x = vectorConvert(res_, size_);
  std::cout << "x: \n" << x << "\n";
  Eigen::MatrixXd H = matrixConvert(H_, size_, H_[0].size());
  std::cout << "H: \n" << H << "\n";
  std::vector<double> node_voltages = getNodeVoltages();
  std::vector<double> currents = nonlinear_.getCurrents(node_voltages);
  Eigen::VectorXd gx = vectorConvert(currents, currents.size());
  std::cout << "g(x): \n" << gx << "\n";
  Eigen::VectorXd s = vectorConvert(temp_b, size_);
  std::cout << "s: \n" << s << "\n";
  Eigen::VectorXd result = G * x + H * gx - s;
  std::cout << "result: \n" << result << "\n";
  std::cout << "result norm: " << result.norm() << "\n";
}

double Matrix::damping(double num_old, double num_new) {
  double diff = num_new - num_old;
  if (diff >= 0) {
    return Gamma / K * std::log(1 + K * std::abs(diff)) + num_old;
  }
  return -Gamma / K * std::log(1 + K * std::abs(diff)) + num_old;
}

void Matrix::dampMethod() {
  std::vector<double> temp;
  for (int i = 0; i < size_; i++) {
    temp.push_back(damping(res_old_[i], res_[i]));
  }
  res_ = temp;
  res_old_ = temp;
}

bool Matrix::isLinear() const { return linear_; }

double Matrix::getNorm() {
  std::vector<std::vector<double>> temp_G;
  std::vector<double> temp_b;
  for (int i = 1; i <= size_; i++) {
    std::vector<double> temp;
    for (int j = 1; j <= size_; j++) {
      temp.push_back(G_[i][j]);
    }
    temp_G.push_back(temp);
    temp_b.push_back(b_ori_[i]);
  }
  Eigen::MatrixXd G = matrixConvert(temp_G, size_, size_);
  Eigen::VectorXd x = vectorConvert(res_, size_);
  Eigen::MatrixXd H = matrixConvert(H_, size_, H_[0].size());
  std::vector<double> node_voltages = getNodeVoltages();
  std::vector<double> currents = nonlinear_.getCurrents(node_voltages);
  Eigen::VectorXd gx = vectorConvert(currents, currents.size());
  Eigen::VectorXd s = vectorConvert(temp_b, size_);
  Eigen::VectorXd result = G * x + H * gx - s;
  return result.norm();
}