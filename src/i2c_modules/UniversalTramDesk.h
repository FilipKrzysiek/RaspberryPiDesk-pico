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
        uint32_t enableTime = 0;
    };
    const uint32_t pressTime = 250;

    struct RawToJoystickBits {
        PairRawBitJoystick leftIndicator{5, 0};
        PairRawBitJoystick disableIndicator{0, 1};
        PairRawBitJoystick rightIndicator{4, 2};
        PairRawBitJoystick leftSwitch{7, 3};
        PairRawBitJoystick disableSwitch{0, 4};
        PairRawBitJoystick rightSwitch{6, 5};
        PairRawBitJoystick unlockDoors{2, 6};
        PairRawBitJoystick lockDoors{1, 7};
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
    std::bitset<16> previousData = 0;

    using UDM = ConfigStorage::UniversalDeskMode;

    UDM deskMode = UDM::Raw;

    uint32_t now = 0;

    inline bool updateValueByTime(bool actualValue, PairRawBitJoystick& bits);

    std::bitset<16> updateRawToJoystickBits() const;

    void updateInTramSimMode(std::bitset<16> &value);

    void updateInCTSTramMode(std::bitset<16> &value);

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
