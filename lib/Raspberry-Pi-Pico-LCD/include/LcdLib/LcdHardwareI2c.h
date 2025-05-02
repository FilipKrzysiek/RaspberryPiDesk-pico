//
// Created by filip on 24.09.2021.
//

#ifndef OMSI_LCDHARDWAREI2C_H
#define OMSI_LCDHARDWAREI2C_H

#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include "LcdHardwareInterface.h"

#define I2C_BACKLIGHT 8
#define I2C_ENABLE 4
#define I2C_RW 2
#define I2C_RS 1


class LcdHardwareI2c: public LcdHardwareInterface {
public:
    LcdHardwareI2c(i2c_inst *i2c, uint8_t i2cAddress, uint *pinI2C);

    LcdHardwareI2c(i2c_inst_t *i2c, uint8_t i2cAddress, uint pinI2C_1, uint pinI2C_2);

    ~LcdHardwareI2c() override;

    void writeData(bool rs, bool rw, uint8_t data) override;

    void writeData4Bit(bool rs, bool rw, uint8_t data) override;

    void enableBackLight() override;

    void disableBackLight() override;

private:
    const uint8_t I2C_ADDRESS;
    i2c_inst_t I2C_intNumber;
    bool backlightIsOn = true;

    void writeDataToI2C(uint8_t data);

};


#endif //OMSI_LCDHARDWAREI2C_H
