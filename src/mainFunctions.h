//
// Created by filip on 28.08.2022.
//

#ifndef RASPBERRYPIDESK_PICO_MAINFUNCTIONS_H
#define RASPBERRYPIDESK_PICO_MAINFUNCTIONS_H

//#include "declarations.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "DashboardMain.h"
#include "i2cFunctions.h"

inline void initialize_i2c() {
    i2c_init(MODULES_I2C, 100 * 1000);

    gpio_set_function(MODULES_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(MODULES_I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(MODULES_I2C_SDA);
    gpio_pull_up(MODULES_I2C_SCL);

    //TODO delete pull up when attach on board
}

inline void initializeGPIO() {
    uint pinsMask = 0;
    pinsMask = pinsMask | (1 << GP_KYB_MASTER_0);
    pinsMask = pinsMask | (1 << GP_KYB_MASTER_1);
    pinsMask = pinsMask | (1 << GP_KYB_SLAVE_0);
    pinsMask = pinsMask | (1 << GP_KYB_SLAVE_1);
    pinsMask = pinsMask | (1 << GP_KYB_SLAVE_2);
    pinsMask = pinsMask | (1 << GP_KYB_SLAVE_3);
    pinsMask = pinsMask | (1 << GP_BUZZER);
    gpio_init_mask(pinsMask);

    gpio_set_dir(GP_BUZZER, GPIO_OUT);
    gpio_put(GP_BUZZER, false);
}

inline DashboardMain *initialize() {
    board_init();
    board_led_write(true);

    initializeGPIO();

    auto *dBoard = new DashboardMain;
    dBoard->writeLine(0, "Initializing");

    initialize_i2c();

    return dBoard;
}

inline void scanAndShowModules(DashboardMain *dBoard) {
    dBoard->writeLine(1, "Scanning modules");

    auto modules = i2c_fun::getConnectedDevices(MODULES_I2C);
    
    if (modules.empty()) {
        dBoard->writeOnDisplay("No connected modules!");
    } else {
        dBoard->writeOnDisplay("Connected devices:");

        unsigned short i = 0;
        for (; i < std::min<uint>(3, modules.size()); ++i) {
            dBoard->writeLine(i + 1, i2c_fun::getNameOfModule(modules[i]));
        }

        while (i < modules.size()){
            unsigned short indx = (i + 2) % 4;
            if (indx == 0) {
                sleep_ms(2500);
                dBoard->writeLine(indx, i2c_fun::getNameOfModule(modules[i]));
            }
            ++i;
        } 
    }
    //TODO test for more than 3 devices
}

inline unsigned short selectMode(DashboardMain *dBoard) {
    dBoard->writeOnDisplay("Chose usbMode:", "1 - Serial", "2 - USB joistick", "3 - USB generic");

    unsigned short val = 0;
    while (val != MODE_SERIAl && val != MODE_JOYSTICK && val != MODE_GENERIC && val != MODE_TESTING) {
        dBoard->readButtons();
        val = dBoard->getButtonsIbisStatusInt();
        sleep_ms(10);
    }

    dBoard->clearDisplay();
    return val;
}

//inline void testMode(DashboardMain *dBoard) {
//
//
//    while (1) {
//        dBoard->readButtons();
//        readButtons = dBoard->getButtonsIbisStatusInt();
//        dBoard->writeOnDisplay(std::to_string(readButtons));
//        sleep_ms(500);
////        continue;
//
//
//        if (board_millis() - timerStart >= 1000) {
//            connectedModules = i2c_fun::getConnectedDevices(MODULES_I2C);
//            timerStart = board_millis();
//            refreshDisplay = true;
//        }
//
//        dBoard->readButtons();
//        readButtons = dBoard->getButtonsIbisStatusInt();
//        if (readButtons == 8 && selectedIndex > 0) {
//            --selectedIndex;
//            refreshDisplay = true;
//        } else if (readButtons == 128 && selectedIndex < connectedModules.size() - 1) {
//            ++selectedIndex;
//            refreshDisplay = true;
//        } else if (readButtons == 32768 || readButtons == 2048 || readButtons == 34816) {
//            dBoard->writeOnDisplay("In leds!");
//            uint8_t addr = 0x21;
//            uint8_t data[3];
//            data[0] = 0x0A;
//            data[1] = 0b00000000;
//            int ret = i2c_write_blocking(i2c1, addr, data, 2, false);
//            if (ret < 0) {
//                return;
//            }
//
//
//            data[0] = 0x00;
//            data[1] = 0b00000111;
//            data[2] = 0b11111111;
//            ret = i2c_write_blocking(i2c1, addr, data, 3, false);
//            if (ret < 0) {
//                return;
//            }
//
//            data[0] = 0x0C;
//            data[1] = 0x00;
//            data[2] = 0x00;
//            ret = i2c_write_blocking(i2c1, addr, data, 3, false);
//            if (ret < 0) {
//                return;
//            }
//
//            bool blinkMode = true;
//            uint8_t counter = 0;
//
//            dBoard->writeOnDisplay("In leds!");
//            data[0] = 0x14;
//
//            while (1) {
//                if (blinkMode) {
//                    if (counter > 5) {
//                        if (data[1] > 0) {
//                            data[1] = 0;
//                        } else
//                            data[1] = 0b11111000;
//                        counter = 0;
//                    }
//                    ++counter;
//                }
//
//                dBoard->readButtons();
//                readButtons = dBoard->getButtonsIbisStatusInt();
//                if (readButtons == 0b1) {
//                    data[1] = (data[1] & 0b10000000) == 0b10000000 ? data[1] & 0b01111000 : data[1] | 0b10000000;
//                } else if (readButtons == 0b10) {
//                    data[1] = (data[1] & 0b01000000) == 0b01000000 ? data[1] & 0b10111000 : data[1] | 0b01000000;
//                } else if (readButtons == 0b100) {
//                    data[1] = (data[1] & 0b00100000) == 0b00100000 ? data[1] & 0b11011000 : data[1] | 0b00100000;
//                } else if (readButtons == 0b1000) {
//                    data[1] = (data[1] & 0b00010000) == 0b00010000 ? data[1] & 0b11101000 : data[1] | 0b00010000;
//                } else if (readButtons == 0b10000) {
//                    data[1] = (data[1] & 0b00001000) == 0b00001000 ? data[1] & 0b11110000 : data[1] | 0b00001000;
//                } else if (readButtons == 0b100000)
//                    blinkMode = !blinkMode;
//
//
//                ret = i2c_write_blocking(i2c1, addr, data, 2, false);
//                if (ret < 0) {
//                    return;
//                }
//
//                sleep_ms(200);
//            }
//        }
//
//        if (refreshDisplay) {
//            testModeRefreshDisplay(dBoard, connectedModules, selectedIndex);
//            refreshDisplay = false;
//        }
//
//        sleep_ms(200);
//    }
//}

#endif //RASPBERRYPIDESK_PICO_MAINFUNCTIONS_H
