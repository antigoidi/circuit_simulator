#include "matrix.h"

Matrix::Matrix(const Parser &parser) {
  std::cout << "Start building Matrix\n";
  EdgeMap v_map = parser.getEdges(V);
  EdgeMap i_map = parser.getEdges(I);
  EdgeMap r_map = parser.getEdges(R);
  auto node_count = parser.getNodeCount();
  std::cout << "Node count: " << node_count << std::endl;
  int sz = node_count + 1 + parser.getCurrentEdgesSize();
  size_ = sz - 1;
  std::cout << "Size: " << sz << std::endl;
  std::vector<std::vector<double>> A(sz, std::vector<double>(sz, 0));
  std::vector<double> b(sz, 0);
  x_.resize(size_);
  for (int i = 1; i <= node_count; i++) {
    x_[i - 1] = "V(" + std::to_string(i) + ")";
  }
  for (auto it = v_map.begin(); it != v_map.end(); ++it) {
    auto current_idx =
        node_count + 1 +
        parser.getCurrentEdgeIndex(it->second.from_, it->second.to_);
    x_[current_idx - 1] = "I(V" + std::to_string(it->second.index_) + ")";
    voltageStamp(it->second.from_, it->second.to_, current_idx,
                 it->second.value_, A, b);
  }
  for (auto it = i_map.begin(); it != i_map.end(); ++it) {
    auto current_idx =
        node_count + 1 +
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
        node_count + 1 +
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
  std::cout << "Building Matrix done\n";

  A_.clear();
  b_.clear();
  for (int i = 1; i < sz; i++) {
    std::vector<double> temp;
    for (int j = 1; j < sz; j++) {
      temp.push_back(A[i][j]);
    }
    A_.push_back(temp);
    b_.push_back(b[i]);
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

std::ostream &operator<<(std::ostream &os, Matrix &mat) {
  os << "Ax = b :\n";
  for (int i = 0; i < mat.size_; i++) {
    for (int j = 0; j < mat.size_; j++) {
      os << mat.A_[i][j] << " ";
    }
    os << " | ";
    os << mat.x_[i];
    os << " | ";
    os << mat.b_[i] << "\n";
  }
  return os;
}