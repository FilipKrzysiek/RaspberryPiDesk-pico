//
// Created by Filip on 28.04.2025.
//

#include "ConfigStorage.h"

#include "hardware/i2c.h"


std::optional<uint8_t> ConfigStorage::getAdjusterMinimumLevel() const {
    return readByte(0);
}

std::optional<uint8_t> ConfigStorage::getAdjusterMaximumLevel() const {
    return readByte(1);
}

std::optional<uint8_t> ConfigStorage::getAdjusterEmergencyBrakeLevel() const {
    return readByte(2);
}

std::optional<uint8_t> ConfigStorage::getAdjusterMiddleLowerLevel() const {
    return readByte(3);
}

std::optional<uint8_t> ConfigStorage::getAdjusterMiddleUpperLevel() const {
    return readByte(4);
}

std::optional<ConfigStorage::UniversalDeskMode> ConfigStorage::getUniversalTramDeskMode() const {
    auto byteOpt = readByte(5);
    if (byteOpt) {
        uint8_t mode = byteOpt.value() & 0b0000'0011;
        return static_cast<UniversalDeskMode>(mode);
    } else {
        return {};
    }
}

bool ConfigStorage::setAdjusterMinimumLevel(uint8_t value) {
    return writeByte(0, value);
}

bool ConfigStorage::setAdjusterMaximumLevel(uint8_t value) {
    return writeByte(1, value);
}

bool ConfigStorage::setAdjusterEmergencyBrakeLevel(uint8_t value) {
    return writeByte(2, value);
}

bool ConfigStorage::setAdjusterMiddleLowerLevel(uint8_t value) {
    return writeByte(3, value);
}

bool ConfigStorage::setAdjusterMiddleUpperLevel(uint8_t value) {
    return writeByte(4, value);
}

bool ConfigStorage::setUniversalTramDeskMode(ConfigStorage::UniversalDeskMode value) {
    if (value == UniversalDeskMode::Raw) {
        return writeByte(5, 0);
    } else if (value == UniversalDeskMode::TramSim) {
        return writeByte(5, 1);
    } else if (value == UniversalDeskMode::CTSTram) {
        return writeByte(5, 2);
    }

    return false;
}

bool ConfigStorage::setUniversalTramDeskModeC(uint8_t value) {
    uint8_t mode = static_cast<uint8_t>(value);
    bool success = writeBit(5, 0, mode & 1);
    return writeBit(5, 1, (mode >> 1) & 1) && success;
}

std::optional<uint8_t> ConfigStorage::readByte(uint8_t byte) const {
    uint8_t readData = 0;

    int status = i2c_write_blocking(LCD_I2C, EEPROM_I2C_ADDR, &byte, 1, true);
    int status1 = i2c_read_blocking(LCD_I2C, EEPROM_I2C_ADDR, &readData, 1, false);

    if (status != PICO_ERROR_GENERIC && status1 != PICO_ERROR_GENERIC)
        return {readData};
    else
        return {};
}

bool ConfigStorage::writeByte(uint8_t byte, uint8_t value) {
    uint8_t writeData[2] = {byte, value};

    int status = i2c_write_blocking(LCD_I2C, EEPROM_I2C_ADDR, writeData, 2, false);

    return status != PICO_ERROR_GENERIC && status != PICO_ERROR_TIMEOUT;
}

std::optional<bool> ConfigStorage::readBit(uint8_t byte, uint8_t bit) const {
    auto readData = readByte(byte);
    if (readData.has_value()) {
        auto readBitData = readData.value();
        return (readBitData >> bit & 1 ) == 1;
    } else {
        return {};
    }
}

bool ConfigStorage::writeBit(uint8_t byte, uint8_t bit, bool value) {
    auto readData = readByte(byte);
    if (readData.has_value()) {
        auto readBitData = readData.value();

        if (value) {
            readBitData |= (1 << bit);
        } else {
            readBitData &= ~(1 << bit);
        }

        return writeByte(byte, readBitData);
    } else {
        return false;
    }
}

//TODO find why other bytes are changing
//TODO change mode not working

//TODO writebit and rwdbit works incorrect
