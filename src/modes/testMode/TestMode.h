//
// Created by filip on 30.10.2022.
//

#ifndef RASPBERRYPIDESK_PICO_TESTMODE_H
#define RASPBERRYPIDESK_PICO_TESTMODE_H

#include <vector>
#include <bitset>
#include "DashboardMain.h"
#include "i2cFunctions.h"
#include "I_module.hpp"

class TestMode {
    DashboardMain *dBoard;
    std::vector <uint> connectedModules;
    uint8_t selectedIndex = 0;
    uint readButtons = 0;
    bool flgRefreshDisplay = true;

    void refreshDisplayConnected();

    void refreshConnectedModules();

    std::string getNameOfI2CModules(const uint &indx);

    void buttonAction();

    [[noreturn]] void ibisButtonsTM();

    [[noreturn]] void moduleTM();

public:
    [[noreturn]] explicit TestMode(DashboardMain *dBoard);
};


#endif //RASPBERRYPIDESK_PICO_TESTMODE_H
