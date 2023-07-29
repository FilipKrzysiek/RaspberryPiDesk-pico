//
// Created by filip on 24.09.2021.
//

#include "LcdLib/LcdHardwareI2c.h"

LcdHardwareI2c::LcdHardwareI2c(i2c_inst *i2c, uint8_t i2cAddress, uint *pinI2C) : LcdHardwareI2c(i2c, i2cAddress,
                                                                                                pinI2C[0],
                                                                                                pinI2C[1]) {}

LcdHardwareI2c::LcdHardwareI2c(i2c_inst_t *i2c, uint8_t i2cAddress, uint pinI2C_1, uint pinI2C_2) : I2C_ADDRESS(
        i2cAddress), I2C_NUM(*i2c) {
    PIN_I2C[0] = pinI2C_1;
    PIN_I2C[1] = pinI2C_2;

    i2c_init(i2c, 100 * 1000);
    gpio_set_function(PIN_I2C[0], GPIO_FUNC_I2C);
    gpio_set_function(PIN_I2C[1], GPIO_FUNC_I2C);
    gpio_pull_up(PIN_I2C[0]);
    gpio_pull_up(PIN_I2C[1]);
}

void LcdHardwareI2c::writeData(bool rs, bool rw, uint8_t data) {
    uint8_t dataToSend = data & 0xF0 | (backlightIsOn ? I2C_BACKLIGHT : 0) | (rs ? I2C_RS : 0) | (rw ? I2C_RW : 0);
    writeDataToI2C(dataToSend);

    dataToSend = dataToSend & 0x0F | ((data & 0x0F) << 4);
    writeDataToI2C(dataToSend);
}

void LcdHardwareI2c::enableBackLight() {
    backlightIsOn = true;
    writeData(false, false, 0);
    //TODO Check is this work OK
}

void LcdHardwareI2c::disableBackLight() {
    backlightIsOn = false;
    writeData(false, false, 0);
}

void LcdHardwareI2c::writeDataToI2C(uint8_t data) {
    i2c_write_blocking(&I2C_NUM, I2C_ADDRESS, &data, 1, false);
    sleep_us(commandSleepTime);
    data = data | I2C_ENABLE;
    i2c_write_blocking(&I2C_NUM, I2C_ADDRESS, &data, 1, false);
    sleep_us(commandSleepTime);
    data = data & ~I2C_ENABLE;
    i2c_write_blocking(&I2C_NUM, I2C_ADDRESS, &data, 1, false);
    sleep_us(commandSleepTime);
}

LcdHardwareI2c::~LcdHardwareI2c() {

}

void LcdHardwareI2c::writeData4Bit(bool rs, bool rw, uint8_t data) {
    data = data << 4 | (backlightIsOn ? I2C_BACKLIGHT : 0) | (rs ? I2C_RS : 0) | (rw ? I2C_RW : 0);
    writeDataToI2C(data);
}
