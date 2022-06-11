#ifndef EDGE_H_
#define EDGE_H_
#include <sstream>
#include <string>

enum Element {
  V,  // voltage source
  I,  // current source
  R,  // resistor
  C,  // capacitor
  L,  // inductor
  D,  // diode
  QN, // npn BJT
  QP, // pnp BJT
  MN, // n_channel MOSFET
  MP, // p_channel MOSFET
  NONE
};

inline std::string convert(Element &type) {
  switch (type) {
  case V:
    return "V";
    break;
  case I:
    return "I";
    break;
  case R:
    return "R";
    break;
  case C:
    return "C";
    break;
  case L:
    return "L";
    break;
  case D:
    return "D";
    break;
  case QN:
    return "QN";
    break;
  case QP:
    return "QP";
    break;
  case MN:
    return "MN";
    break;
  case MP:
    return "MP";
    break;
  default:
    break;
  }
  return "NONE";
}

struct Edge {
  unsigned int from_;
  unsigned int to_;
  Element type_;
  unsigned int index_;
  double value_;
  std::string option_;
  std::string getString() {
    std::stringstream ss;
    ss << convert(type_) << index_ << " " << from_ << " " << to_ << " "
       << value_ << " " << option_ << "\n";
    return ss.str();
  }
};

// only used for QN, QM, MN, MP
struct TEdge {
  unsigned int from_;
  unsigned int mid_;
  unsigned int to_;
  Element type_;
  unsigned int index_;
  double value_;
  std::string option_;
  std::string getString() {
    std::stringstream ss;
    ss << convert(type_) << index_ << " " << from_ << " " << mid_ << " " << to_
       << " " << value_ << " " << option_ << "\n";
    return ss.str();
  }
};

#endif