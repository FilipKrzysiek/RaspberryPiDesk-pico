//
// Created by filip on 30.10.2022.
//

#include <bsp/board.h>
#include "UniversalBusDesk.hpp"

bool UniversalBusDesk::initialize() {
    data[0] = 0x0A;
    data[1] = 0b00000000;
    int ret = i2c_write_blocking(MODULES_I2C, addr, data, 2, false);
    if (ret < 0) {
        initialized = false;
        return false;
    }

    data[0] = 0x00;
    data[1] = 0b00000111;
    data[2] = 0b11111111;
    ret = i2c_write_blocking(MODULES_I2C, addr, data, 3, false);
    if (ret < 0) {
        initialized = false;
        return false;
    }

    data[0] = 0x0C;
    data[1] = 0b00000111;
    data[2] = 0b11111111;
    ret = i2c_write_blocking(MODULES_I2C, addr, data, 3, false);
    if (ret < 0) {
        initialized = false;
        return false;
    }

    data[0] = 0x02;
    data[1] = 0xFF;
    data[2] = 0xFF;
    ret = i2c_write_blocking(MODULES_I2C, addr, data, 3, false);
    if (ret < 0) {
        initialized = false;
        return false;
    }

    initialized = true;
    return true;
}

bool UniversalBusDesk::reset() {
    return initialize();
}

bool UniversalBusDesk::communicate(const uint8_t *toDevice) {
    ledState = *toDevice;
    for (uint8_t i = 0; i < 3; i++ ) {
        if (!writeToDevice()) {
            sleep_ms(1);
            reset();
            continue;
        }

        if (!getButtonsState()) {
            sleep_ms(1);
            reset();
            continue;
        }

        return true;
    }
    return false;
}

void UniversalBusDesk::testModeLed(uint readButtons) {
    if (readButtons == 1) {
        ledState = (ledState & 0b10000000) == 0b10000000 ? ledState & 0b01111000 : ledState | 0b10000000;
    } else if (readButtons == 0b10) {
        ledState = (ledState & 0b01000000) == 0b01000000 ? ledState & 0b10111000 : ledState | 0b01000000;
    } else if (readButtons == 0b100) {
        ledState = (ledState & 0b00100000) == 0b00100000 ? ledState & 0b11011000 : ledState | 0b00100000;
    } else if (readButtons == 0b1000) {
        ledState = (ledState & 0b00010000) == 0b00010000 ? ledState & 0b11101000 : ledState | 0b00010000;
    } else if (readButtons == 0b10000) {
        ledState = (ledState & 0b00001000) == 0b00001000 ? ledState & 0b11110000 : ledState | 0b00001000;
    } else if (readButtons == 32) {
        flgBlink = !flgBlink;
        ledState = 0;
    }

    if (flgBlink) {
        if (board_millis() - timerStart >= 1000) {
            timerStart = board_millis();

            ledState = ~ledState;
        }
    }
}

bool UniversalBusDesk::writeToDevice() {
    data[0] = 0x14;
    data[1] = ledState;
    return i2c_write_blocking(MODULES_I2C, addr, data, 2, false) != PICO_ERROR_GENERIC;
}

bool UniversalBusDesk::getButtonsState() {
    data[0] = 0x12;
    int ret = i2c_write_blocking(MODULES_I2C, addr, data, 1, true);
    if (ret == PICO_ERROR_GENERIC) {
        return false;
    }

    ret = i2c_read_blocking(MODULES_I2C, addr, data, 2, false);
    if (ret == PICO_ERROR_GENERIC) {
        return false;
    }

    buttonsState = data[0];
    buttonsState += (data[1] << 8);
    return true;
}

UniversalBusDesk::UniversalBusDesk() : I_Module(0x21) {}

uint8_t *UniversalBusDesk::getDataFromDevice() {
    return reinterpret_cast<uint8_t*>(&buttonsState);
}

unsigned int UniversalBusDesk::testMode(DashboardMain *dBoard) {
    testModeLed(dBoard->getButtonsIbisStatusInt());

    if(!communicate(&ledState)) {
        dBoard->writeLine(1, "Error on communication!");
        dBoard->writeLine(2, "");
    } else {
        dBoard->writeLine(1, "B: " + std::bitset<9>(buttonsState).to_string());
        dBoard->writeLine(2, "LED: " + std::bitset<5>(ledState).to_string());
    }

    return buttonsState;
}
