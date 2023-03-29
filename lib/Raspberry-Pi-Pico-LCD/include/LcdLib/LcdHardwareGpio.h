//
// Created by filip on 24.09.2021.
//

#ifndef OMSI_LCDHARDWAREGPIO_H
#define OMSI_LCDHARDWAREGPIO_H

#ifndef USE_4_BIT_DATA_BUS
#define USE_4_BIT_DATA_BUS 1
#endif //USE_4_BIT_DATA_BUS

#include <pico/stdlib.h>
#include "LcdHardwareInterface.h"

class LcdHardwareGpio : public LcdHardwareInterface {
public:
    LcdHardwareGpio(const uint &pinReadWrite, const uint &pinCommandData, const uint &pinEnable, const uint *pinData,
                    const uint &backlight);

    LcdHardwareGpio(const uint &pinReadWrite, const uint &pinCommandData, const uint &pinEnable, const uint *pinData);

    LcdHardwareGpio(const uint &pinCommandData, const uint &pinEnable, const uint *pinData);

    LcdHardwareGpio(const uint &pinCommandData, const uint &pinEnable, const uint *pinData, const uint &backlight);

#if USE_4_BIT_DATA_BUS == 1

    LcdHardwareGpio(const uint &pinReadWrite, const uint &pinCommandData, const uint &pinEnable, const uint &pinData0,
                    const uint &pinData1, const uint &pinData2, const uint &pinData3);

    LcdHardwareGpio(const uint &pinCommandData, const uint &pinEnable, const uint &pinData0, const uint &pinData1,
                    const uint &pinData2, const uint &pinData3);

    LcdHardwareGpio(const uint &pinReadWrite, const uint &pinCommandData, const uint &pinEnable, const uint &pinData0,
                    const uint &pinData1, const uint &pinData2, const uint &pinData3, const uint &backlight);

#else
    LcdHardwareGpio(const uint &pinReadWrite, const uint &pinComandData, const uint &pinEnable, const uint &pinData0,
            const uint &pinData1, const uint &pinData2, const uint &pinData3, const uint &pinData4, const uint &pinData5,
            const uint &pinData6, const uint &pinData7);

    LcdHardwareGpio(const uint &pinCommandData, const uint &pinEnable, const uint &pinData0, const uint &pinData1,
            const uint &pinData2, const uint &pinData3, const uint &pinData4, const uint &pinData5, const uint &pinData6,
            const uint &pinData7);

    LcdHardwareGpio(const uint &pinReadWrite, const uint &pinCommandData, const uint &pinEnable, const uint &pinData0,
                    const uint &pinData1, const uint &pinData2, const uint &pinData3, const uint &pinData4, const uint &pinData5,
                    const uint &pinData6, const uint &pinData7, const uint &backlight);
#endif

    ~LcdHardwareGpio() override;

    void setBacklightPin(const uint &backlightPin);

    void writeData(bool rs, bool rw, uint8_t data) override;

    void writeData4Bit(bool rs, bool rw, uint8_t data) override;

    void enableBackLight() override;

    void disableBackLight() override;

private:
#if USE_4_BIT_DATA_BUS == 1
    uint PIN_DATA[4];
#else
    uint PIN_DATA[8];
#endif

    bool rwPinConnected = true;
    const uint PIN_READ_WRITE = 255;
    const uint PIN_COMMAND_DATA;
    const uint PIN_ENABLE;
    uint PIN_BACKLIGHT = 255;

    void enablePulse();

};


#endif //OMSI_LCDHARDWAREGPIO_H
