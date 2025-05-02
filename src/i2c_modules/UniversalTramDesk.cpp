//
// Created by Filip on 12.04.2025.
//

#include "UniversalTramDesk.h"

uint16_t UniversalTramDesk::updateBit(PairRawBitJoystick bits, uint8_t value) {
    return ((value >> bits.rawBit) & 1) << bits.joystickBit;
}

uint16_t UniversalTramDesk::updateBitAndUpdateTime(PairRawBitJoystick &bits, uint8_t value) {
    uint8_t actualValue = (value >> bits.rawBit) & 1;

    updateValueByTime(actualValue, bits);

    return actualValue << bits.joystickBit;
}

void UniversalTramDesk::updateValueByTime(uint8_t &actualValue, PairRawBitJoystick &bits) {
    if (actualValue == ((previousData >> bits.joystickBit) & 1)) {
        if (actualValue && now - bits.enableTime > pressTime) {
            actualValue = 0;
        }
    } else {
        if (actualValue) {
            bits.enableTime = now;
        }
    }
}

uint16_t UniversalTramDesk::updateRawToJoystickBits() {
    uint16_t result{0};

    result += updateBitAndUpdateTime(RTJB.leftIndicator, data[0]);
    result += updateBitAndUpdateTime(RTJB.rightIndicator, data[0]);
    result += updateBitAndUpdateTime(RTJB.leftSwitch, data[0]);
    result += updateBitAndUpdateTime(RTJB.rightSwitch, data[0]);
    result += updateBit(RTJB.unlockDoors, data[0]);
    result += updateBit(RTJB.lockDoors, data[0]);

    return result;
}

void UniversalTramDesk::updateInTramSimMode(uint16_t &value) {
    uint8_t actualValueTmp = 1;

    if (~(data[0] >> RTJB.leftIndicator.rawBit) & 1 && ~(data[0] >> RTJB.rightIndicator.rawBit) & 1) {
        actualValueTmp = 1;
        updateValueByTime(actualValueTmp, RTJB.disableIndicator);
        value += actualValueTmp;
    } else {
        actualValueTmp = 0;
        updateValueByTime(actualValueTmp, RTJB.disableIndicator);
    }

    if (~(data[0] >> RTJB.leftSwitch.rawBit) & 1 && ~(data[0] >> RTJB.rightSwitch.rawBit) & 1) {
        actualValueTmp = 1;
        updateValueByTime(actualValueTmp, RTJB.disableSwitch);
        value += actualValueTmp;
    } else {
        actualValueTmp = 0;
        updateValueByTime(actualValueTmp, RTJB.disableSwitch);
    }
}

void UniversalTramDesk::updateInCTSTramMode(uint16_t &value) {
    uint8_t actualValueTmp = 1;

    if (~(data[0] >> RTJB.leftIndicator.rawBit) & 1 && ~(data[0] >> RTJB.rightIndicator.rawBit) & 1 && lastIndicator != Disabled) {
        actualValueTmp = 1;
        if (lastIndicator == Left) {
            updateValueByTime(actualValueTmp, RTJB.rightIndicator);
        } else {
            updateValueByTime(actualValueTmp, RTJB.leftIndicator);
        }
    }

    if (~(data[0] >> RTJB.leftSwitch.rawBit) & 1 && ~(data[0] >> RTJB.rightSwitch.rawBit) & 1 && lastSwitch != Disabled) {
        actualValueTmp = 1;
        if (lastSwitch == Left) {
            updateValueByTime(actualValueTmp, RTJB.rightSwitch);
        } else {
            updateValueByTime(actualValueTmp, RTJB.leftSwitch);
        }
        value += actualValueTmp;
    }

    if (data[0] >> RTJB.leftIndicator.rawBit) {
        lastIndicator = Left;
    } else if (data[0] >> RTJB.rightIndicator.rawBit) {
        lastIndicator = Right;
    } else {
        lastIndicator = Disabled;
    }

    if (data[0] >> RTJB.leftSwitch.rawBit) {
        lastSwitch = Left;
    } else if (data[0] >> RTJB.rightSwitch.rawBit) {
        lastSwitch = Right;
    } else {
        lastSwitch = Disabled;
    }
}

void UniversalTramDesk::updateMode() {
    auto mode = storage.getUniversalTramDeskMode();
    if (mode.has_value()) {
        deskMode = mode.value();

        if (deskMode == UDM::Error) {
            deskMode = UDM::Raw;
        }
    } else {
        deskMode = UDM::Raw;
    }
}

UniversalTramDesk::UniversalTramDesk():
    I_Module(0x27, 0, 1) {
}

bool UniversalTramDesk::initialize() {
    updateMode();

    // Set default values to 0 without first bit, because this make communication error
    data[0] = 0x11000001;
    data[1] = 0b00000000;


    // initialized = i2c_write_blocking(MODULES_I2C, addr, data, 1, false) != PICO_ERROR_GENERIC;

    initialized = i2c_read_blocking(MODULES_I2C, addr, data, 2, false) != PICO_ERROR_GENERIC;
    return initialized;
}

bool UniversalTramDesk::reset() {
    return initialize();
}

bool UniversalTramDesk::communicate(const uint8_t *toDevice) {
    int ret = i2c_read_blocking(MODULES_I2C, addr, data, 2, false);
    if (ret == PICO_ERROR_GENERIC) {
        return false;
    }
    data[0] = ~data[0];

    if (!flgIsInTestMode) {
        now = get_absolute_time();
        auto newData = updateRawToJoystickBits();

        if (deskMode == UDM::TramSim) {
            updateInTramSimMode(newData);
        } else if (deskMode == UDM::CTSTram) {
            updateInCTSTramMode(newData);
        } else /* RAW or error */{

        }

        previousData = newData;
        previousRaw = data[0];

        *reinterpret_cast<uint16_t*>(data[0]) = newData;
    }

    return true;
}

uint8_t * UniversalTramDesk::getDataFromDevice() {
    return data;
}

unsigned int UniversalTramDesk::testMode(DashboardMain *dBoard) {
    flgIsInTestMode = true;
    if (communicate(nullptr)) {
        dBoard->writeLine(1, "RAW:" + std::bitset<16>(data[0]).to_string());
    } else {
        dBoard->writeLine(1, "Error on communication!");
    }

    dBoard->writeLine(2, "Change mode: A,B,C");

    if (deskMode == UDM::TramSim) {
        dBoard->writeLine(3, "TramSim");
    } else if (deskMode == UDM::CTSTram) {
        dBoard->writeLine(3, "CTSTram");
    } else if (deskMode == UDM::Raw) {
        dBoard->writeLine(3, "RAW");
    } else {
        dBoard->writeLine(3, "Error on update mode!");
    }

    dBoard->readButtons();
    auto buttons = dBoard->getButtonsIbisChanged();

    if (buttons == BASE_KYB_A) {
        dBoard->writeLine(3, "Tramsim mode!");
        if (!storage.setUniversalTramDeskMode(UDM::TramSim)) {
            deskMode = UDM::Error;
        } else {
            // updateMode();
        }
    } else if (buttons == BASE_KYB_B) {
        if (!storage.setUniversalTramDeskMode(UDM::CTSTram)) {
            deskMode = UDM::Error;
        } else {
            updateMode();
        }
    } else if (buttons == BASE_KYB_C) {
        if (!storage.setUniversalTramDeskMode(UDM::Raw)) {
            deskMode = UDM::Error;
        } else {
            updateMode();
        }
    }


    return data[0];
}
