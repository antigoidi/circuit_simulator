#ifndef DEVICE_H_
#define DEVICE_H_
#include <vector>

class Device {
public:
  Device() = default;
  virtual void update(std::vector<double> &voltages){};
  virtual std::vector<double> getCurrents(std::vector<double> &voltages) {
    return {};
  }
  virtual void elementStamp(std::vector<std::vector<double>> &A,
                            std::vector<double> &b,
                            std::vector<int> &node_pos) {}
  virtual ~Device() = default;
};

#endif