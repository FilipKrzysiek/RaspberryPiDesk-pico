//
// Created by Filip on 28.04.2025.
//

#ifndef CONFIGSTORAGE_H
#define CONFIGSTORAGE_H
#include "declarations.hpp"
#include <optional>


class ConfigStorage {
public:
    enum class UniversalDeskMode {
        Raw = 0,
        TramSim = 1,
        CTSTram = 2,
        Error
    };

    std::optional<uint8_t> getAdjusterMinimumLevel() const;

    std::optional<uint8_t> getAdjusterMaximumLevel() const;

    std::optional<uint8_t> getAdjusterEmergencyBrakeLevel() const;

    std::optional<uint8_t> getAdjusterMiddleLowerLevel() const;

    std::optional<uint8_t> getAdjusterMiddleUpperLevel() const;

    std::optional<UniversalDeskMode> getUniversalTramDeskMode() const;

    bool setAdjusterMinimumLevel(uint8_t value);

    bool setAdjusterMaximumLevel(uint8_t value);

    bool setAdjusterEmergencyBrakeLevel(uint8_t value);

    bool setAdjusterMiddleLowerLevel(uint8_t value);

    bool setAdjusterMiddleUpperLevel(uint8_t value);

    bool setUniversalTramDeskMode(UniversalDeskMode value);

    std::optional<uint8_t> readByte(uint8_t byte) const;

// private:

    bool writeByte(uint8_t byte, uint8_t value);

    std::optional<bool> readBit(uint8_t byte, uint8_t bit) const;

    bool writeBit(uint8_t byte, uint8_t bit, bool value);
};



#endif //CONFIGSTORAGE_H
