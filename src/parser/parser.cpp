#include "parser.h"

bool Parser::readFile(std::ifstream &myFile) {
  std::string myText;
  node_count_ = 0;
  while (std::getline(myFile, myText)) {
    // std::cout << myText << std::endl;
    Element type = NONE;
    Edge edge;
    TEdge tedge;
    if (strParser(myText, type, edge, tedge)) {
      switch (type) {
      case V:
        if (V_.find(edge.index_) != V_.end()) {
          myFile.close();
          return false;
        }
        V_[edge.index_] = edge;
        break;
      case I:
        if (I_.find(edge.index_) != I_.end()) {
          myFile.close();
          return false;
        }
        I_[edge.index_] = edge;
        break;
      case R:
        if (R_.find(edge.index_) != R_.end()) {
          myFile.close();
          return false;
        }
        R_[edge.index_] = edge;
        break;
      case C:
        if (C_.find(edge.index_) != C_.end()) {
          myFile.close();
          return false;
        }
        C_[edge.index_] = edge;
        break;
      case L:
        if (L_.find(edge.index_) != L_.end()) {
          myFile.close();
          return false;
        }
        L_[edge.index_] = edge;
        break;
      case D:
        if (D_.find(edge.index_) != D_.end()) {
          myFile.close();
          return false;
        }
        D_[edge.index_] = edge;
        break;
      case QN:
        if (QN_.find(tedge.index_) != QN_.end()) {
          myFile.close();
          return false;
        }
        QN_[tedge.index_] = tedge;
        break;
      case QP:
        if (QP_.find(tedge.index_) != QP_.end()) {
          myFile.close();
          return false;
        }
        QP_[tedge.index_] = tedge;
        break;
      case MN:
        if (MN_.find(tedge.index_) != MN_.end()) {
          myFile.close();
          return false;
        }
        MN_[tedge.index_] = tedge;
        break;
      case MP:
        if (MP_.find(tedge.index_) != MP_.end()) {
          myFile.close();
          return false;
        }
        MP_[tedge.index_] = tedge;
        break;
      default:
        break;
      }
    } else {
      std::cout << "input file format not supported" << std::endl;
      myFile.close();
      return false;
    }
  }
  myFile.close();
  return true;
}

bool isNumber(const std::string &s) {
  for (char ch : s) {
    if (!std::isdigit(ch)) {
      return false;
    }
  }
  return true;
}

bool valid(std::string &word, Element ele, Element &type, Edge &edge) {
  type = ele;
  std::string str = word.substr(1, word.length());
  if (isNumber(str)) {
    edge.type_ = ele;
    edge.index_ = std::stoi(str);
    return true;
  }
  return false;
}

bool Parser::strParser(std::string &myText, Element &type, Edge &edge,
                       TEdge &tedge) {
  if (myText.empty()) {
    return NONE;
  }
  std::stringstream ss(myText);
  std::string word;
  int count = 0;
  while (ss >> word) {
    if (count == 0) {
      switch (word[0]) {
      case 'V':
      case 'v':
        if (!valid(word, V, type, edge)) {
          return false;
        }
        break;
      case 'I':
      case 'i':
        if (!valid(word, I, type, edge)) {
          return false;
        }
        break;
      case 'R':
      case 'r':
        if (!valid(word, R, type, edge)) {
          return false;
        }
        break;
      case 'C':
      case 'c':
        if (!valid(word, C, type, edge)) {
          return false;
        }
        break;
      case 'L':
      case 'l':
        if (!valid(word, L, type, edge)) {
          return false;
        }
        break;
      case 'D':
      case 'd':
        if (!valid(word, D, type, edge)) {
          return false;
        }
        break;
      case 'Q':
      case 'q': {
        if (word[1] == 'N' || word[1] == 'n') {
          type = QN;
          std::string str = word.substr(2, word.length() - 1);
          if (isNumber(str)) {
            tedge.type_ = type;
            tedge.index_ = std::stoi(str);
          } else {
            return false;
          }
        } else if (word[1] == 'P' || word[1] == 'p') {
          type = QP;
          std::string str = word.substr(2, word.length() - 1);
          if (isNumber(str)) {
            tedge.type_ = type;
            tedge.index_ = std::stoi(str);
          } else {
            return false;
          }
        } else {
          return false;
        }
        break;
      }
      case 'M':
      case 'm': {
        if (word[1] == 'N' || word[1] == 'n') {
          type = MN;
          std::string str = word.substr(2, word.length() - 1);
          if (isNumber(str)) {
            tedge.type_ = type;
            tedge.index_ = std::stoi(str);
          } else {
            return false;
          }
        } else if (word[1] == 'P' || word[1] == 'p') {
          type = MP;
          std::string str = word.substr(2, word.length() - 1);
          if (isNumber(str)) {
            tedge.type_ = type;
            tedge.index_ = std::stoi(str);
          } else {
            return false;
          }
        } else {
          return false;
        }
        break;
      }
      default:
        return false;
        break;
      }
      count++;
    } else {
      if (type == QN || type == QP || type == MN || type == MP) {
        if (count == 1) {
          int from = std::stoi(word);
          tedge.from_ = from;
        } else if (count == 2) {
          int mid = std::stoi(word);
          tedge.mid_ = mid;
        } else if (count == 3) {
          int to = std::stoi(word);
          tedge.to_ = to;
        } else if (count == 4) {
          double value = std::stod(word);
          tedge.value_ = value;
        } else if (count == 5) {
          std::string option = word;
          tedge.option_ = option;
        } else {
          return false;
        }
        count++;
      } else {
        if (count == 1) {
          int from = std::stoi(word);
          edge.from_ = from;
        } else if (count == 2) {
          int to = std::stoi(word);
          edge.to_ = to;
        } else if (count == 3) {
          double value = std::stod(word);
          edge.value_ = value;
        } else if (count == 4) {
          std::string option = word;
          edge.option_ = option;
        } else {
          return false;
        }
        count++;
      }
    }
  }
  if (type == QN || type == QP || type == MN || type == MP) {
    if (count >= 5) {
      updateNodeCount2(tedge);
      return true;
    }
    return false;
  }
  if (count >= 4) {
    if (edge.type_ == V || edge.option_ == "G2") {
      current_edges_.push_back(std::make_pair(edge.from_, edge.to_));
    }
    updateNodeCount1(edge);
    return true;
  }
  return false;
}

EdgeMap Parser::getEdges(Element type) const {
  switch (type) {
  case V:
    return V_;
    break;
  case I:
    return I_;
    break;
  case R:
    return R_;
    break;
  case C:
    return C_;
    break;
  case L:
    return L_;
    break;
  case D:
    return D_;
    break;
  default:
    EdgeMap emptyMap;
    return emptyMap;
    break;
  }
}

TEdgeMap Parser::getTEdges(Element type) const {
  switch (type) {
  case QN:
    return QN_;
    break;
  case QP:
    return QP_;
    break;
  case MN:
    return MN_;
    break;
  case MP:
    return MP_;
    break;
  default:
    TEdgeMap emptyMap;
    return emptyMap;
    break;
  }
}

std::ostream &operator<<(std::ostream &os, EdgeMap &edgemap) {
  auto iter = edgemap.begin();
  for (; iter != edgemap.end(); ++iter) {
    os << iter->second.getString();
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, TEdgeMap &tedgemap) {
  auto iter = tedgemap.begin();
  for (; iter != tedgemap.end(); ++iter) {
    os << iter->second.getString();
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, Parser &parser) {
  os << parser.V_;
  os << parser.I_;
  os << parser.R_;
  os << parser.C_;
  os << parser.L_;
  os << parser.D_;
  os << parser.QN_;
  os << parser.QP_;
  os << parser.MN_;
  os << parser.MP_;
  return os;
}

int Parser::getNodeCount() const { return node_count_; }

std::vector<std::pair<unsigned int, unsigned int>>
Parser::getCurrentEdges() const {
  return current_edges_;
}

int Parser::getCurrentEdgeIndex(unsigned int from, unsigned int to) const {
  for (int i = 0; i < current_edges_.size(); i++) {
    if (current_edges_[i].first == from && current_edges_[i].second == to) {
      return i;
    }
  }
  return 0;
}

int Parser::getCurrentEdgesSize() const { return current_edges_.size(); }

void Parser::updateNodeCount1(Edge &edge) {
  int max = edge.from_ > edge.to_ ? edge.from_ : edge.to_;
  if (node_count_ < max) {
    node_count_ = max;
  }
}

void Parser::updateNodeCount2(TEdge &tedge) {
  int temp = tedge.from_ > tedge.to_ ? tedge.from_ : tedge.to_;
  int max = tedge.mid_ > temp ? tedge.mid_ : temp;
  if (node_count_ < max) {
    node_count_ = max;
  }
}
