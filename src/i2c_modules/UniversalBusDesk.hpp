//
// Created by filip on 30.10.2022.
//

#ifndef RASPBERRYPIDESK_PICO_UNIVERSALBUSDESK_HPP
#define RASPBERRYPIDESK_PICO_UNIVERSALBUSDESK_HPP

#include "I_module.hpp"

class UniversalBusDesk : public I_Module {
    bool flgBlink = true;
    uint8_t ledState = 0;
    uint16_t buttonsState = 0;
    uint8_t data[3]{};
    uint32_t timerStart = 0;

    bool writeToDevice();

    bool getButtonsState();

public:
    /**
     * Size of data send to device (in bytes)
     */
    static constexpr size_t D_SIZE_TD = 1;

    /**
     * Size of data receive from device (in bytes)
     */
    static constexpr size_t D_SIZE_FD = 2;

    explicit UniversalBusDesk();

    bool initialize() override;

    bool reset() override;

    bool communicate(const uint8_t *toDevice) override;

    uint8_t *getDataFromDevice() override;

    void testModeLed(uint readButtons);

    unsigned int testMode(DashboardMain *dBoard) override;
};




#endif //RASPBERRYPIDESK_PICO_UNIVERSALBUSDESK_HPP
