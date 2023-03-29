//
// Created by Filip on 21.09.2021.
//

#ifndef RASPBERRYPIDESK_PICO_DECLARATIONS_HPP
#define RASPBERRYPIDESK_PICO_DECLARATIONS_HPP

#include <map>
#include "pico/stdlib.h"

#define MODE_SERIAl 1
#define MODE_JOYSTICK 2
#define MODE_GENERIC 4
#define MODE_TESTING 8

#define GP_KYB_MASTER_0 6
#define GP_KYB_MASTER_1 7
#define GP_KYB_SLAVE_0 2
#define GP_KYB_SLAVE_1 3
#define GP_KYB_SLAVE_2 4
#define GP_KYB_SLAVE_3 5

constexpr i2c_inst_t *LCD_I2C = i2c0;
#define LCD_I2C_ADR 0x27
#define LCD_I2C_SDA 0
#define LCD_I2C_SCL 1

constexpr i2c_inst_t *MODULES_I2C = i2c1;
#define MODULES_I2C_SDA 14
#define MODULES_I2C_SCL 15

#define CHAR_ADDR_AR_UP 0
#define CHAR_ADDR_AR_DOWN 1

#define LCD_ENABLE 0x04

const uint IBIS_BUTTONS_I2C_FANTOM = 0x00;

constexpr uint MODULES_LIST[] = {0x21, 0x00};
const std::map<uint, const char *> MODULES_NAME_LIST = {{0x21,                    "Bus desk basic"},
                                                        {IBIS_BUTTONS_I2C_FANTOM, "IBIS.md buttons"}};
constexpr uint GP_KYB_SLAVE[] = {GP_KYB_SLAVE_0, GP_KYB_SLAVE_1, GP_KYB_SLAVE_2, GP_KYB_SLAVE_3};

const uint8_t CHAR_DOTS_AR_UP[]{
        0b00100,
        0b01110,
        0b11111,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100};

const uint8_t CHAR_DOTS_AR_DOWN[]{
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b11111,
        0b01110,
        0b00100};


//static_assert(LCD_I2C != MODULES_I2C, "LCD and modules should not use the same i2c line");
//TODO static assert for checking LCD_I2C != MODULES_I2C
#endif //RASPBERRYPIDESK_PICO_DECLARATIONS_HPP
