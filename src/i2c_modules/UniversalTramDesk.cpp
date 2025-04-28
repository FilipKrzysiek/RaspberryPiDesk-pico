//
// Created by Filip on 12.04.2025.
//

#include "UniversalTramDesk.h"

UniversalTramDesk::UniversalTramDesk():
    I_Module(0x27, 0, 1) {
}

bool UniversalTramDesk::initialize() {
    // Set default values to 0 without first bit, because this make communication error
    data[0] = 0x11000001;
    data[1] = 0b00000000;


    // initialized = i2c_write_blocking(MODULES_I2C, addr, data, 1, false) != PICO_ERROR_GENERIC;

    initialized = i2c_read_blocking(MODULES_I2C, addr, data, 2, false) != PICO_ERROR_GENERIC;
    return initialized;
}

bool UniversalTramDesk::reset() {
    initialize();
}

bool UniversalTramDesk::communicate(const uint8_t *toDevice) {
    int ret = i2c_read_blocking(MODULES_I2C, addr, data, 2, false);
    if (ret == PICO_ERROR_GENERIC) {
        return false;
    }

    data[0] = ~data[0];
    return true;
}

uint8_t * UniversalTramDesk::getDataFromDevice() {
    return data;
}

unsigned int UniversalTramDesk::testMode(DashboardMain *dBoard) {
    if (communicate(nullptr)) {
        dBoard->writeLine(1, "RAW: " + std::bitset<8>(data[0]).to_string() + " " + std::bitset<8>(data[1]).to_string());
    } else {
        dBoard->writeLine(1, "Error on communication!");
    }

    dBoard->readButtons();
    auto buttons = dBoard->getButtonsIbisChanged();

    if (buttons & 1) {
        data[0] = 0b00000000;
        bool ok = i2c_write_blocking(MODULES_I2C, addr, data, 1, false) != PICO_ERROR_GENERIC;
        dBoard->writeLine(1, "Write 000 " + std::to_string(ok));
        sleep_ms(1000);
    } else if (buttons & 0b10) {
        data[0] = 0b11111111;
        data[1] = 0b11111111;
        bool ok = i2c_write_blocking(MODULES_I2C, addr, data, 2, false) != PICO_ERROR_GENERIC;
        dBoard->writeLine(1, "Write 111 " + std::to_string(ok));
        sleep_ms(1000);
    } else if (buttons & 0b100) {
        data[0] = 0b00000001;
        data[1] = 0b00000001;
        bool ok = i2c_write_blocking(MODULES_I2C, addr, data, 2, false) != PICO_ERROR_GENERIC;
        dBoard->writeLine(1, "Write 111 " + std::to_string(ok));
        sleep_ms(1000);
    }


    return data[0];
}
