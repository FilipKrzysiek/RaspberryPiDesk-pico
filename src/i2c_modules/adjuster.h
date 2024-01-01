//
// Created by Filip on 01.01.2024.
//

#ifndef ADJUSTER_H
#define ADJUSTER_H
#include "I_module.hpp"

class Adjuster : public I_Module  {
  uint8_t adjusterLevel;
public:
  explicit Adjuster() : I_Module(0x48) {}
  bool initialize() override;
  bool reset() override;
  bool communicate(const uint8_t *toDevice) override;
  uint8_t *getDataFromDevice() override;
  unsigned testMode(DashboardMain *dBoard) override;
};



#endif //ADJUSTER_H
