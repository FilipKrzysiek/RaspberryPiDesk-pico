//
// Created by Filip on 01.01.2024.
//

#ifndef GAMEPADMODE_H
#define GAMEPADMODE_H
#include "DashboardMain.h"
#include "I_module.hpp"
#include "i2c_modules/UniversalTramDesk.h"
#include "i2c_modules/adjuster.h"
#include "usb/usbMode.hpp"


class GamePadMode {
    DashboardMain *dBoard;
    // uint16_t statusDBoardButtons = 0;
    // uint8_t statusAdjuster = 127;
    usb_reports::Joystick::report_t *inputStates;
    bool flgTryToInitialize = false;

    Adjuster moduleAdjuster;
    UniversalTramDesk moduleUniversalTramDesk;

    void getDataFromDBoard();

    void getDataFromAdjuster();

    void getDataFromTramDashboard();

public:
    explicit GamePadMode(DashboardMain *d_board);

    void run();
};



#endif //GAMEPADMODE_H
