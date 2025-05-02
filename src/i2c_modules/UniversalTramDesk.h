//
// Created by Filip on 12.04.2025.
//

#ifndef UNIVERSALTRAMDESK_H
#define UNIVERSALTRAMDESK_H
#include "I_module.hpp"


class UniversalTramDesk: public I_Module {
    struct PairRawBitJoystick{
        const uint8_t rawBit;
        const uint8_t joystickBit;
        absolute_time_t enableTime = 0;
    };
    const absolute_time_t pressTime = 500;

    struct RawToJoystickBits {
        PairRawBitJoystick leftIndicator{0b00100000, 1};
        PairRawBitJoystick disableIndicator{0, 2};
        PairRawBitJoystick rightIndicator{0b00010000, 3};
        PairRawBitJoystick leftSwitch{0b10000000, 4};
        PairRawBitJoystick disableSwitch{0, 5};
        PairRawBitJoystick rightSwitch{0b01000000, 6};
        PairRawBitJoystick unlockDoors{0b00001000, 7};
        PairRawBitJoystick lockDoors{0b00000010, 8};
    };

    enum Direction {
        Left = 0,
        Disabled = 1,
        Right = 2,
    };

    Direction lastIndicator = Direction::Disabled;
    Direction lastSwitch = Direction::Disabled;

    RawToJoystickBits RTJB;

    bool flgIsInTestMode = false;

    uint8_t data[2]{};
    uint8_t previousRaw = 0;
    uint16_t previousData = 0;

    using UDM = ConfigStorage::UniversalDeskMode;

    UDM deskMode = UDM::Raw;

    absolute_time_t now = 0;

    inline uint16_t updateBit(PairRawBitJoystick bits, uint8_t value);
    inline uint16_t updateBitAndUpdateTime(PairRawBitJoystick &bits, uint8_t value);

    inline void updateValueByTime(uint8_t &actualValue, PairRawBitJoystick& bits);

    uint16_t updateRawToJoystickBits();

    void updateInTramSimMode(uint16_t &value);

    void updateInCTSTramMode(uint16_t &value);

    void updateMode();
public:
    explicit UniversalTramDesk();

    bool initialize() override;

    bool reset() override;

    bool communicate(const uint8_t *toDevice) override;

    uint8_t *getDataFromDevice() override;

    unsigned int testMode(DashboardMain *dBoard) override;

};



#endif //UNIVERSALTRAMDESK_H
