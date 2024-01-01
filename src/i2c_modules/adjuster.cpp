//
// Created by Filip on 01.01.2024.
//

#include "adjuster.h"
bool Adjuster::initialize() {
  uint8_t controlByte = 0b00000000;
  return i2c_write_blocking(MODULES_I2C, addr, &controlByte, 1, false) !=
         PICO_ERROR_GENERIC;
}
bool Adjuster::reset() { return initialize(); }
bool Adjuster::communicate(const uint8_t *toDevice) {
  uint8_t data[5];
  int ret2 = i2c_read_blocking(MODULES_I2C, addr, &data[0], 5, false);

  if (ret2 == PICO_ERROR_GENERIC) {
    sleep_ms(10);
    reset();
    return false;
  }

  adjusterLevel = (data[1] + data[2] + data[3] + data[4]) / 4;

  return true;
}
uint8_t *Adjuster::getDataFromDevice() {
  return &adjusterLevel;
}
unsigned Adjuster::testMode(DashboardMain *dBoard) {
  if (communicate(nullptr)) {
    dBoard->writeOnDisplay("Adjuster test mocde", "RAW: " + std::to_string(adjusterLevel));
  } else {
    dBoard->writeOnDisplay("Error on communication!");
  }

  //TODO setting min max and add not RAW value
}