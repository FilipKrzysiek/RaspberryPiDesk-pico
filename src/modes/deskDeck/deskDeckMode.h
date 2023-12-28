//
// Created by filip on 3/25/23.
//

#ifndef RASPBERRYPIDESK_PICO_DESKDECKMODE_H
#define RASPBERRYPIDESK_PICO_DESKDECKMODE_H

#include "DashboardMain.h"
#include "DashboardMenu.h"
#include "usb/usbMode.hpp"


class DeskDeckMode {
    DashboardMain *dBoard;
    usbConnectionMode_t usbMode = USB_DESK;
    void (DeskDeckMode::*mode)() = nullptr;
    uint32_t sleepTimer = 0;
    bool screenSleep = false;

    void mainButtons();

    void selectMode();

    void insertingPredefinedText();

    void inventorMode();

    void inventorModeKeyActions(const uint &val);

    void omsiKeyboardMode();

    void writeString1();

    void writeString2();

    void checkConnected();

    void wakeUpScreen();

    void backToMainMode();

public:
    explicit DeskDeckMode(DashboardMain *dBoard);

    usbConnectionMode_t run();
};


#endif //RASPBERRYPIDESK_PICO_DESKDECKMODE_H
