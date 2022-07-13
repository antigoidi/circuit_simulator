#ifndef PARSER_H_
#define PARSER_H_
#include "edge.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using EdgeMap = std::unordered_map<unsigned int, Edge>;
using TEdgeMap = std::unordered_map<unsigned int, TEdge>;

class Parser {
public:
  Parser() = default;
  bool readFile(std::ifstream &myFile);
  bool strParser(std::string &myText, Element &type, Edge &edge, TEdge &tedge);
  EdgeMap getEdges(Element type) const;
  TEdgeMap getTEdges(Element type) const;
  int getNodeCount() const;
  std::vector<std::pair<unsigned int, unsigned int>> getCurrentEdges() const;
  int getCurrentEdgeIndex(unsigned int from, unsigned int to) const;
  int getCurrentEdgesSize() const;
  void updateNodeCount1(Edge &edge);
  void updateNodeCount2(TEdge &tedge);

  bool isLinear() const;
  int getNLECount() const;

  friend std::ostream &operator<<(std::ostream &os, EdgeMap &edgemap);
  friend std::ostream &operator<<(std::ostream &os, TEdgeMap &tedgemap);
  friend std::ostream &operator<<(std::ostream &os, Parser &parser);

private:
  EdgeMap V_;
  EdgeMap I_;
  EdgeMap R_;
  EdgeMap C_;
  EdgeMap L_;
  EdgeMap D_;
  TEdgeMap QN_;
  TEdgeMap QP_;
  TEdgeMap MN_;
  TEdgeMap MP_;
  int node_count_;
  int diode_count_ = 0;
  int tri_count_ = 0;
  std::vector<std::pair<unsigned int, unsigned int>> current_edges_;
};
#endif