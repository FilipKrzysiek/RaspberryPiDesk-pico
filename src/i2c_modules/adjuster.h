//
// Created by Filip on 01.01.2024.
//

#ifndef ADJUSTER_H
#define ADJUSTER_H
#include "I_module.hpp"

class Adjuster : public I_Module {
    uint8_t adjusterLevelRawT[5];
    uint8_t &adjusterLevelRaw = adjusterLevelRawT[0];
    struct FromDevice {
        uint8_t adjusterLevelScaled;
        uint8_t emergencyBrakeOn;
    };

    FromDevice fromDevice;

    uint8_t readBytes = 0;
    const uint8_t minLevel = 21;
    const uint8_t minMid = 139;
    const uint8_t maxMid = 175;
    const uint8_t maxLevel = 251;
    const uint8_t emergencyBrakeLevel = 36;

    void processRawValue();

public:
    explicit Adjuster() : I_Module(0x48, 0, 2) {}
    bool initialize() override;
    bool reset() override;
    bool communicate(const uint8_t *toDevice) override;
    uint8_t *getDataFromDevice() override;
    unsigned testMode(DashboardMain *dBoard) override;
};


#endif // ADJUSTER_H
