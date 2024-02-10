//
// Created by filip on 28.08.2022.
//

#include "pico/multicore.h"
#include "mainFunctions.h"
#include "usb/usbMode.hpp"
#include "usb/usbFunctions.h"
#include "modes/testMode/TestMode.h"
#include "modes/deskDeck/deskDeckMode.h"
#include "modes/gamePadMode/GamePadMode.h"

usbConnectionMode_t usbCMode = USB_NONE;
usb_reports::DeskDeck deskDeckReport;
usb_reports::Joystick joystickReport;

int main() {
    auto *dBoard = initialize();
    scanAndShowModules(dBoard);
    sleep_ms(2500);

    {
        usbCMode = USB_DESK;
        multicore_launch_core1(usb_task);
        DeskDeckMode ddMode(dBoard);
        usbCMode = ddMode.run();
        //TODO devaice not changing dynamic - not detect when change from desk to joystick
        tud_disconnect();
    }

    sleep_ms(300);
    board_led_write(false);

    if (usbCMode == USB_SERIAL) {

        dBoard->writeOnDisplay("NOT IMPLEMENTED!");
    } else if (usbCMode == USB_JOYSTICK) {
        tud_connect();
        dBoard->writeOnDisplay("Joystick mode");

        GamePadMode gpMode(dBoard);
        gpMode.run();
    } else if (usbCMode == USB_GENERIC) {

        dBoard->writeOnDisplay("NOT IMPLEMENTED!");
    } else if (usbCMode == USB_TEST_MODE) {
        TestMode testMode(dBoard);
    } else {
        dBoard->writeOnDisplay("ERROR!");
    }

    return 0;
}
