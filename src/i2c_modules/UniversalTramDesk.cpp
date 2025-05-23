//
// Created by Filip on 12.04.2025.
//

#include "UniversalTramDesk.h"


bool UniversalTramDesk::updateValueByTime(bool actualValue, PairRawBitJoystick &bits) {
    if (actualValue == true && ((previousRaw >> bits.rawBit) & 1) == 0) {
        bits.enableTime = now;
    } else {
        if (now - bits.enableTime > pressTime) {
            actualValue = false;
        } else {
            actualValue = true;
        }
    }

    return actualValue;
}

std::bitset<16> UniversalTramDesk::updateRawToJoystickBits() const {
    std::bitset<16> result;
    result[RTJB.leftIndicator.joystickBit] = (data[0] >> RTJB.leftIndicator.rawBit) & 1;
    result[RTJB.rightIndicator.joystickBit] = (data[0] >> RTJB.rightIndicator.rawBit) & 1;
    result[RTJB.leftSwitch.joystickBit] = (data[0] >> RTJB.leftSwitch.rawBit) & 1;
    result[RTJB.rightSwitch.joystickBit] = (data[0] >> RTJB.rightSwitch.rawBit) & 1;
    result[RTJB.unlockDoors.joystickBit] = (data[0] >> RTJB.unlockDoors.rawBit) & 1;
    result[RTJB.lockDoors.joystickBit] = (data[0] >> RTJB.lockDoors.rawBit) & 1;

    return result;
}

void UniversalTramDesk::updateInTramSimMode(std::bitset<16> &value) {
    if (~(data[0] >> RTJB.leftIndicator.rawBit) & 1 && ~(data[0] >> RTJB.rightIndicator.rawBit) & 1) {
        value[RTJB.disableIndicator.joystickBit] = true;
    } else {
        value[RTJB.disableIndicator.joystickBit] = false;
    }

    if (~(data[0] >> RTJB.leftSwitch.rawBit) & 1 && ~(data[0] >> RTJB.rightSwitch.rawBit) & 1) {
        value[RTJB.disableSwitch.joystickBit] = true;
    } else {
        value[RTJB.disableSwitch.joystickBit] = false;
    }
}

void UniversalTramDesk::updateInCTSTramMode(std::bitset<16> &value) {
    if (~(data[0] >> RTJB.leftIndicator.rawBit) & 1 && ~(data[0] >> RTJB.rightIndicator.rawBit) & 1 && lastIndicator != Disabled) {
        if (lastIndicator == Left) {
            value[RTJB.rightIndicator.joystickBit] = updateValueByTime(true, RTJB.rightIndicator);
        } else {
            value[RTJB.leftIndicator.joystickBit] = updateValueByTime(true, RTJB.leftIndicator);
        }
    } else {
        value[RTJB.leftIndicator.joystickBit] = updateValueByTime(value[RTJB.leftIndicator.joystickBit], RTJB.leftIndicator);
        value[RTJB.rightIndicator.joystickBit] = updateValueByTime(value[RTJB.rightIndicator.joystickBit], RTJB.rightIndicator);
    }

    if (~(data[0] >> RTJB.leftSwitch.rawBit) & 1 && ~(data[0] >> RTJB.rightSwitch.rawBit) & 1 && lastSwitch != Disabled) {
        if (lastSwitch == Left) {
            value[RTJB.rightSwitch.joystickBit] = updateValueByTime(true, RTJB.rightSwitch);
        } else {
            value[RTJB.leftSwitch.joystickBit] = updateValueByTime(true, RTJB.leftSwitch);
        }
    } else {
        value[RTJB.leftSwitch.joystickBit] = updateValueByTime(value.test(RTJB.leftSwitch.joystickBit), RTJB.leftSwitch);
        value[RTJB.rightSwitch.joystickBit] = updateValueByTime(value.test(RTJB.rightSwitch.joystickBit), RTJB.rightSwitch);
    }

    if (data[0] >> RTJB.leftIndicator.rawBit & 1) {
        lastIndicator = Left;
    } else if (data[0] >> RTJB.rightIndicator.rawBit & 1) {
        lastIndicator = Right;
    } else {
        lastIndicator = Disabled;
    }

    if (data[0] >> RTJB.leftSwitch.rawBit & 1) {
        lastSwitch = Left;
    } else if (data[0] >> RTJB.rightSwitch.rawBit & 1) {
        lastSwitch = Right;
    } else {
        lastSwitch = Disabled;
    }
}

void UniversalTramDesk::updateMode() {
    auto mode = storage.getUniversalTramDeskMode();
    if (mode.has_value()) {
        deskMode = mode.value();

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
        now = to_ms_since_boot(get_absolute_time());
        auto newData = updateRawToJoystickBits();

        if (deskMode == UDM::TramSim) {
            updateInTramSimMode(newData);
        } else if (deskMode == UDM::CTSTram) {
            updateInCTSTramMode(newData);
        } else /* RAW or error */{

        }

        previousData = newData;
        previousRaw = data[0];

        data[0] = newData.to_ulong() & 0xFF;
        data[1] = (newData.to_ulong() >> 8) & 0xFF;
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
        if (!storage.setUniversalTramDeskMode(UDM::TramSim)) {
            deskMode = UDM::Error;
        } else {
            sleep_ms(10);
            updateMode();
        }
    } else if (buttons == BASE_KYB_B) {
        if (!storage.setUniversalTramDeskMode(ConfigStorage::UniversalDeskMode::CTSTram)) {
            deskMode = UDM::Error;
        } else {
            sleep_ms(10);
            updateMode();
        }
    } else if (buttons == BASE_KYB_C) {
        if (!storage.setUniversalTramDeskMode(UDM::Raw)) {
            deskMode = UDM::Error;
        } else {
            sleep_ms(10);
            updateMode();
        }
    }


    return data[0];
}
