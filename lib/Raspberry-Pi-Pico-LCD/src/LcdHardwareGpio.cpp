//
// Created by filip on 24.09.2021.
//

#include "LcdLib/LcdHardwareGpio.h"

LcdHardwareGpio::LcdHardwareGpio(const uint &pinReadWrite, const uint &pinCommandData, const uint &pinEnable,
                                 const uint *pinData, const uint &backlight) : LcdHardwareGpio(pinReadWrite,
                                                                                               pinCommandData,
                                                                                               pinEnable, pinData) {
    PIN_BACKLIGHT = backlight;
}

LcdHardwareGpio::LcdHardwareGpio(const uint &pinCommandData, const uint &pinEnable, const uint *pinData,
                                 const uint &backlight) : LcdHardwareGpio(pinCommandData, pinEnable, pinData) {
    PIN_BACKLIGHT = backlight;
}

LcdHardwareGpio::LcdHardwareGpio(const uint &pinReadWrite, const uint &pinCommandData, const uint &pinEnable,
                                 const uint *pinData) :
        PIN_READ_WRITE(pinReadWrite), PIN_COMMAND_DATA(pinCommandData), PIN_ENABLE(pinEnable) {
    rwPinConnected = true;
    PIN_DATA[0] = pinData[0];
    PIN_DATA[1] = pinData[1];
    PIN_DATA[2] = pinData[2];
    PIN_DATA[3] = pinData[3];

#if USE_4_BIT_DATA_BUS != 1
    PIN_DATA[4] = pinData[4];
    PIN_DATA[5] = pinData[5];
    PIN_DATA[6] = pinData[6];
    PIN_DATA[7] = pinData[7];
#endif
}

LcdHardwareGpio::LcdHardwareGpio(const uint &pinCommandData, const uint &pinEnable, const uint *pinData) :
        PIN_COMMAND_DATA(pinCommandData), PIN_ENABLE(pinEnable) {
    rwPinConnected = false;
    PIN_DATA[0] = pinData[0];
    PIN_DATA[1] = pinData[1];
    PIN_DATA[2] = pinData[2];
    PIN_DATA[3] = pinData[3];

#if USE_4_BIT_DATA_BUS != 1
    PIN_DATA[4] = pinData[4];
    PIN_DATA[5] = pinData[5];
    PIN_DATA[6] = pinData[6];
    PIN_DATA[7] = pinData[7];
#endif
}


#if USE_4_BIT_DATA_BUS == 1

LcdHardwareGpio::LcdHardwareGpio(const uint &pinReadWrite, const uint &pinCommandData, const uint &pinEnable,
                                 const uint &pinData0,
                                 const uint &pinData1, const uint &pinData2, const uint &pinData3) :
        PIN_READ_WRITE(pinReadWrite), PIN_COMMAND_DATA(pinCommandData), PIN_ENABLE(pinEnable) {
    rwPinConnected = true;
    PIN_DATA[0] = pinData0;
    PIN_DATA[1] = pinData1;
    PIN_DATA[2] = pinData2;
    PIN_DATA[3] = pinData3;
}

LcdHardwareGpio::LcdHardwareGpio(const uint &pinCommandData, const uint &pinEnable, const uint &pinData0,
                                 const uint &pinData1,
                                 const uint &pinData2, const uint &pinData3) :
        PIN_COMMAND_DATA(pinCommandData), PIN_ENABLE(pinEnable) {
    rwPinConnected = false;
    PIN_DATA[0] = pinData0;
    PIN_DATA[1] = pinData1;
    PIN_DATA[2] = pinData2;
    PIN_DATA[3] = pinData3;
}

LcdHardwareGpio::LcdHardwareGpio(const uint &pinReadWrite, const uint &pinCommandData, const uint &pinEnable,
                                 const uint &pinData0, const uint &pinData1, const uint &pinData2, const uint &pinData3,
                                 const uint &backlight) : LcdHardwareGpio(pinReadWrite, pinCommandData, pinEnable,
                                                                          pinData0, pinData1, pinData2, pinData3) {
    PIN_BACKLIGHT = backlight;
}

#else

LcdHardwareGpio::LcdHardwareGpio(const uint &pinReadWrite, const uint &pinComandData, const uint &pinEnable, const uint &pinData0,
                 const uint &pinData1, const uint &pinData2, const uint &pinData3, const uint &pinData4, const uint &pinData5,
                 const uint &pinData6, const uint &pinData7) :
                 PIN_READ_WRITE(pinReadWrite), PIN_COMMAND_DATA(pinComandData), PIN_ENABLE(pinEnable) {
    rwPinConnected = true;
    PIN_DATA[0] = pinData0;
    PIN_DATA[1] = pinData1;
    PIN_DATA[2] = pinData2;
    PIN_DATA[3] = pinData3;
    PIN_DATA[4] = pinData4;
    PIN_DATA[5] = pinData5;
    PIN_DATA[6] = pinData6;
    PIN_DATA[7] = pinData7;
}

LcdHardwareGpio::LcdHardwareGpio(const uint &pinCommandData, const uint &pinEnable, const uint &pinData0, const uint &pinData1,
                 const uint &pinData2, const uint &pinData3, const uint &pinData4, const uint &pinData5,
                 const uint &pinData6, const uint &pinData7) :
                 PIN_COMMAND_DATA(pinCommandData), PIN_ENABLE(pinEnable) {
    rwPinConnected = false;
    PIN_DATA[0] = pinData0;
    PIN_DATA[1] = pinData1;
    PIN_DATA[2] = pinData2;
    PIN_DATA[3] = pinData3;
    PIN_DATA[4] = pinData4;
    PIN_DATA[5] = pinData5;
    PIN_DATA[6] = pinData6;
    PIN_DATA[7] = pinData7;
}

LcdHardwareGpio::LcdHardwareGpio(const uint &pinReadWrite, const uint &pinCommandData, const uint &pinEnable,
                                 const uint &pinData0, const uint &pinData1, const uint &pinData2, const uint &pinData3,
                                 const uint &pinData4, const uint &pinData5, const uint &pinData6, const uint &pinData7,
                                 const uint &backlight) : LcdHardwareGpio(pinReadWrite, pinCommandData, pinEnable,
                                                                          pinData0, pinData1, pinData2, pinData3,
                                                                          pinData4, pinData5, pinData6, pinData7) {
    PIN_BACKLIGHT = backlight;
}
#endif

void LcdHardwareGpio::writeData(bool rs, bool rw, uint8_t data) {
    gpio_put(PIN_COMMAND_DATA, rs);
    if (rwPinConnected) gpio_put(PIN_READ_WRITE, rw);
#if USE_4_BIT_DATA_BUS == 1
    gpio_put(PIN_DATA[0], (data >> 4) & 1);
    gpio_put(PIN_DATA[1], (data >> 5) & 1);
    gpio_put(PIN_DATA[2], (data >> 6) & 1);
    gpio_put(PIN_DATA[2], (data >> 7) & 1);
    enablePulse();

    gpio_put(PIN_DATA[0], data & 1);
    gpio_put(PIN_DATA[1], (data >> 1) & 1);
    gpio_put(PIN_DATA[2], (data >> 2) & 1);
    gpio_put(PIN_DATA[3], (data >> 3) & 1);
    enablePulse();
#else
    gpio_put(PIN_DATA[0], data & 1);
    gpio_put(PIN_DATA[1], (data >> 1) & 1);
    gpio_put(PIN_DATA[2], (data >> 2) & 1);
    gpio_put(PIN_DATA[3], (data >> 3) & 1);
    gpio_put(PIN_DATA[4], (data >> 4) & 1);
    gpio_put(PIN_DATA[5], (data >> 5) & 1);
    gpio_put(PIN_DATA[6], (data >> 6) & 1);
    gpio_put(PIN_DATA[7], (data >> 7) & 1);
    enablePulse();
#endif
}

void LcdHardwareGpio::enableBackLight() {
    if (PIN_BACKLIGHT != 255)
        gpio_put(PIN_BACKLIGHT, true);
}

void LcdHardwareGpio::disableBackLight() {
    if (PIN_BACKLIGHT != 255)
        gpio_put(PIN_BACKLIGHT, false);

}

void LcdHardwareGpio::setBacklightPin(const uint &backlightPin) {
    PIN_BACKLIGHT = backlightPin;
}

void LcdHardwareGpio::enablePulse() {
    gpio_put(PIN_ENABLE, false);
    sleep_us(commandSleepTime);
    gpio_put(PIN_ENABLE, true);
    sleep_us(commandSleepTime);
    gpio_put(PIN_ENABLE, false);
    sleep_us(commandSleepTime);
}

LcdHardwareGpio::~LcdHardwareGpio() {

}

void LcdHardwareGpio::writeData4Bit(bool rs, bool rw, uint8_t data) {
    gpio_put(PIN_COMMAND_DATA, rs);
    if (rwPinConnected) gpio_put(PIN_READ_WRITE, rw);
#if USE_4_BIT_DATA_BUS == 1
    gpio_put(PIN_DATA[0], data & 1);
    gpio_put(PIN_DATA[1], (data >> 1) & 1);
    gpio_put(PIN_DATA[2], (data >> 2) & 1);
    gpio_put(PIN_DATA[3], (data >> 3) & 1);
    enablePulse();
#else
    gpio_put(PIN_DATA[4], data & 1);
    gpio_put(PIN_DATA[5], (data >> 1) & 1);
    gpio_put(PIN_DATA[6], (data >> 2) & 1);
    gpio_put(PIN_DATA[7], (data >> 3) & 1);
    enablePulse();
#endif
}
