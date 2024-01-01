//
// Created by Filip on 21.09.2021.
//

#ifndef RASPBERRYPIDESK_PICO_DECLARATIONS_HPP
#define RASPBERRYPIDESK_PICO_DECLARATIONS_HPP

#include <map>
#include "pico/stdlib.h"

#define GP_BUZZER 17
#define GP_BUZZER_TIME 25

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

constexpr uint MODULES_LIST[] = {0x21, 0x48, 0x00};
const std::map<uint, const char *> MODULES_NAME_LIST = {{0x21,                    "Bus desk basic"},
                                                        {0x48,                    "Adjuster"},
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

// -----------------------------------------------

constexpr uint16_t BASE_KYB_1 = 0b0000000000000001;
constexpr uint16_t BASE_KYB_2 = 0b0000000000000010;
constexpr uint16_t BASE_KYB_3 = 0b0000000000000100;
constexpr uint16_t BASE_KYB_A = 0b0000000000001000;
constexpr uint16_t BASE_KYB_4 = 0b0000000000010000;
constexpr uint16_t BASE_KYB_5 = 0b0000000000100000;
constexpr uint16_t BASE_KYB_6 = 0b0000000001000000;
constexpr uint16_t BASE_KYB_B = 0b0000000010000000;
constexpr uint16_t BASE_KYB_7 = 0b0000000100000000;
constexpr uint16_t BASE_KYB_8 = 0b0000001000000000;
constexpr uint16_t BASE_KYB_9 = 0b0000010000000000;
constexpr uint16_t BASE_KYB_C = 0b0000100000000000;
constexpr uint16_t BASE_KYB_STAR = 0b0001000000000000;
constexpr uint16_t BASE_KYB_0 = 0b0010000000000000;
constexpr uint16_t BASE_KYB_HASH = 0b0100000000000000;
constexpr uint16_t BASE_KYB_D = 0b1000000000000000;

//static_assert(LCD_I2C != MODULES_I2C, "LCD and modules should not use the same i2c line");
//TODO static assert for checking LCD_I2C != MODULES_I2C
#endif //RASPBERRYPIDESK_PICO_DECLARATIONS_HPP
