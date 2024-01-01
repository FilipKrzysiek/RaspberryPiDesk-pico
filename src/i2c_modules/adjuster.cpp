//
// Created by Filip on 01.01.2024.
//

#include "adjuster.h"

void Adjuster::processRawValue() {
  uint16_t tmp = adjusterLevelRawT[1] + adjusterLevelRawT[2] + adjusterLevelRawT[3] + adjusterLevelRawT[4];
  adjusterLevelRaw = tmp == 0 ? 0 : tmp / 4;

  //TODO add more complex calculations
  if (adjusterLevelRaw > 118) {
    adjusterLevelScaled = 127.5 / (max - mid) * (adjusterLevelRaw - mid) + 127.5;
  } else {
    adjusterLevelScaled = 127.5 / (mid - min) * (adjusterLevelRaw - min);
  }
}

bool Adjuster::initialize() {
  uint8_t controlByte = 0b00000000;
  return i2c_write_blocking(MODULES_I2C, addr, &controlByte, 1, false) !=
         PICO_ERROR_GENERIC;
}
bool Adjuster::reset() { return initialize(); }
bool Adjuster::communicate(const uint8_t *toDevice) {
  // initialize();
  readBytes = i2c_read_blocking(MODULES_I2C, addr, &adjusterLevelRawT[0], 5, false);

  if (readBytes == PICO_ERROR_GENERIC) {
    sleep_ms(10);
    reset();
    return false;
  }

  processRawValue();

  return true;
}
uint8_t *Adjuster::getDataFromDevice() {
  return &adjusterLevelScaled;
}
unsigned Adjuster::testMode(DashboardMain *dBoard) {
  if (communicate(nullptr)) {
    dBoard->writeLine(1, "RAW: " + std::to_string(adjusterLevelRaw) + " (" + std::to_string(adjusterLevelScaled) + ")");
    dBoard->writeLine(3, "Readed bytes: " + std::to_string(readBytes));
  } else {
    dBoard->writeLine(1, "Error on communication!");
    dBoard->writeLine(3, "Readed bytes: " + std::to_string(readBytes));
  }

  return adjusterLevelScaled;
  //TODO setting min max and add not RAW value
}